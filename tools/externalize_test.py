#!/usr/bin/env python3
"""
Externalize embedded Test methods to dedicated TesterClass.

Transforms Pattern B (embedded) to Pattern A (TesterClass) across three files:
  1. Header (*.h): Remove embedded Test from class body, add TesterClass after class
  2. Test header (*_test.h): Change Foo::Test to FooTester::Test
  3. Registration (.cpp): Change TDDContainer<Foo> to TDDContainer<FooTester>
"""

import re
import sys
import argparse
from pathlib import Path
from dataclasses import dataclass


@dataclass
class ClassDescriptor:
    """Describes a class to be transformed."""
    class_name: str
    tester_name: str  # Usually class_name + "Tester"
    header: str  # Path to src/include/.../foo.h
    test_header: str  # Path to src/include/.../foo_test.h
    registration_cpp: str  # Path to src/tests/.../foo.cpp
    use_struct: bool = False  # Use 'struct' instead of 'class' for templated types
    namespace: str = "TempLat"
    template_params: str = ""  # Template parameters if class is templated, e.g. "size_t NDim, typename T"


def extract_template_params(header_content: str, class_name: str) -> str:
    """
    Extract template parameters from a class definition.
    Returns the template parameter list as a string, e.g., "size_t NDim, typename T"
    Returns empty string if class is not templated.
    """
    # Match: template<...> class ClassName or template<...> struct ClassName
    pattern = rf'template\s*<([^>]+)>\s*(?:class|struct)\s+{re.escape(class_name)}\b'
    match = re.search(pattern, header_content)
    if match:
        return match.group(1).strip()
    return ""


def transform_header(content: str, class_name: str, tester_name: str,
                     use_struct: bool = False, namespace: str = "TempLat",
                     template_params: str = "") -> str:
    """
    Transform header file:
    1. Remove embedded #ifdef TEMPLATTEST block from class body
    2. Add TesterClass before namespace closing brace

    For templated classes, TesterClass inherits template parameters from production class.
    """

    # Step 1: Remove embedded block from inside the class
    embedded_pattern = (
        r'\n\s*#ifdef TEMPLATTEST\s*\n\s*public:\s*\n\s*'
        r'static inline void Test\(TDDAssertion &tdd\);\s*\n\s*#endif\s*\n'
    )
    content = re.sub(embedded_pattern, '\n', content, count=1)

    # Also handle alternative pattern
    if '#ifdef TEMPLATTEST' in content:
        simple_pattern = (
            r'#ifdef TEMPLATTEST\s*\n\s*public:\s*\n\s*'
            r'static inline void Test\(TDDAssertion &tdd\);\s*\n\s*#endif'
        )
        content = re.sub(simple_pattern, '', content, count=1)

    # Step 2: Add TesterClass before namespace closing
    namespace_close = f'}} // namespace {namespace}'

    if namespace_close in content:
        tester_class_type = 'struct' if use_struct else 'class'

        # Build TesterClass declaration
        if template_params:
            tester_block = (
                f'\n#ifdef TEMPLATTEST\n'
                f'template<{template_params}>\n'
                f'  {tester_class_type} {tester_name}\n'
                f'  {{\n'
                f'  public:\n'
                f'    static inline void Test(TDDAssertion &tdd);\n'
                f'  }};\n'
                f'#endif\n'
            )
        else:
            tester_block = (
                f'\n#ifdef TEMPLATTEST\n'
                f'  {tester_class_type} {tester_name}\n'
                f'  {{\n'
                f'  public:\n'
                f'    static inline void Test(TDDAssertion &tdd);\n'
                f'  }};\n'
                f'#endif\n'
            )

        content = content.replace(namespace_close, tester_block + namespace_close)
    else:
        # Fallback: look for just closing brace and comment
        fallback_pattern = f'}} // namespace'
        if fallback_pattern in content:
            tester_class_type = 'struct' if use_struct else 'class'

            if template_params:
                tester_block = (
                    f'\n#ifdef TEMPLATTEST\n'
                    f'template<{template_params}>\n'
                    f'  {tester_class_type} {tester_name}\n'
                    f'  {{\n'
                    f'  public:\n'
                    f'    static inline void Test(TDDAssertion &tdd);\n'
                    f'  }};\n'
                    f'#endif\n'
                )
            else:
                tester_block = (
                    f'\n#ifdef TEMPLATTEST\n'
                    f'  {tester_class_type} {tester_name}\n'
                    f'  {{\n'
                    f'  public:\n'
                    f'    static inline void Test(TDDAssertion &tdd);\n'
                    f'  }};\n'
                    f'#endif\n'
                )

            content = content.replace(fallback_pattern, tester_block + fallback_pattern)

    return content


def transform_test_header(content: str, class_name: str, tester_name: str,
                          namespace: str = "TempLat", use_struct: bool = False,
                          template_params: str = "") -> str:
    """
    Transform test header file:
    Change function qualifier from Foo::Test to FooTester::Test
    Handles both non-templated and templated classes.

    For non-templated: "inline void NS::Foo::Test" -> "inline void NS::FooTester::Test"
    For templated: "template<...> inline void NS::Foo<...>::Test" -> "template<...> inline void NS::FooTester<...>::Test"
    """
    # First try: simple non-templated case
    old_qualifier = f'{namespace}::{class_name}::Test('
    new_qualifier = f'{namespace}::{tester_name}::Test('
    if old_qualifier in content:
        content = content.replace(old_qualifier, new_qualifier, 1)
        return content

    # Second try: templated case - keep template parameters, just change class and instantiation
    if template_params:
        # Pattern: "template <SAME_PARAMS> inline void NS::ClassName<SAME_PARAMS>::Test"
        # Replace with: "template <SAME_PARAMS> inline void NS::TesterName<SAME_PARAMS>::Test"
        # We need to preserve the actual template parameters in the specialization

        # Extract the template parameters from the test method to find what to replace
        # Pattern: template<...> inline void NS::ClassName<...>::Test
        templated_pattern = (
            r'(template\s*<[^>]*>)\s+inline\s+void\s+' +
            re.escape(f'{namespace}::{class_name}') +
            r'(<[^>]*>::Test\s*\()'
        )
        # Replacement: keep template declaration, change class name and specialization
        # We'll use a function to do the replacement properly
        def replace_templated(match):
            template_decl = match.group(1)  # e.g., "template<size_t NDim, typename T>"
            rest = match.group(2)  # e.g., "<size_t NDim, typename T>::Test("
            return f'{template_decl} inline void {namespace}::{tester_name}{rest}'

        new_content = re.sub(templated_pattern, replace_templated, content, count=1)
        if new_content != content:
            return new_content

    return content


def transform_registration_cpp(content: str, class_name: str, tester_name: str,
                               namespace: str = "TempLat") -> str:
    """
    Transform registration .cpp file:
    Change TDDContainer<Foo> to TDDContainer<FooTester>
    Also handles templated classes: TDDContainer<Foo<...>> to TDDContainer<FooTester<...>>

    For templated classes with template specialization, preserve the template arguments.
    """
    # First try: simple non-templated case
    old_container = f'TDDContainer<{namespace}::{class_name}>'
    new_container = f'TDDContainer<{namespace}::{tester_name}>'
    if old_container in content:
        content = content.replace(old_container, new_container, 1)
        return content

    # Second try: templated case with template specialization
    # Pattern: TDDContainer<NS::ClassName<ARGS>>
    # Replace with: TDDContainer<NS::TesterName<ARGS>>
    templated_pattern = (
        r'TDDContainer<' +
        re.escape(f'{namespace}::{class_name}') +
        r'<([^>]*)>>'
    )
    replacement = f'TDDContainer<{namespace}::{tester_name}<\\1>>'

    new_content = re.sub(templated_pattern, replacement, content)
    if new_content != content:
        return new_content

    return content


def apply_transformation(descriptor: ClassDescriptor, dry_run: bool = False) -> dict:
    """
    Apply transformation to all three files for a given class descriptor.
    Returns a dict with status for each file.

    Auto-detects template parameters if not provided.
    """
    results = {
        'class': descriptor.class_name,
        'header': None,
        'test_header': None,
        'registration_cpp': None,
        'errors': []
    }

    # Auto-detect template parameters if not provided
    template_params = descriptor.template_params
    if not template_params:
        try:
            header_path = Path(descriptor.header)
            if header_path.exists():
                header_content = header_path.read_text()
                template_params = extract_template_params(header_content, descriptor.class_name)
        except Exception:
            pass

    # Transform header
    try:
        header_path = Path(descriptor.header)
        if not header_path.exists():
            results['errors'].append(f"Header not found: {descriptor.header}")
        else:
            content = header_path.read_text()
            new_content = transform_header(
                content, descriptor.class_name, descriptor.tester_name,
                descriptor.use_struct, descriptor.namespace, template_params
            )
            if content != new_content:
                results['header'] = 'transformed'
                if not dry_run:
                    header_path.write_text(new_content)
            else:
                results['header'] = 'no_change'
    except Exception as e:
        results['errors'].append(f"Header error: {e}")

    # Transform test header
    try:
        test_header_path = Path(descriptor.test_header)
        if not test_header_path.exists():
            results['errors'].append(f"Test header not found: {descriptor.test_header}")
        else:
            content = test_header_path.read_text()
            new_content = transform_test_header(
                content, descriptor.class_name, descriptor.tester_name,
                descriptor.namespace, descriptor.use_struct, template_params
            )
            if content != new_content:
                results['test_header'] = 'transformed'
                if not dry_run:
                    test_header_path.write_text(new_content)
            else:
                results['test_header'] = 'no_change'
    except Exception as e:
        results['errors'].append(f"Test header error: {e}")

    # Transform registration cpp
    try:
        cpp_path = Path(descriptor.registration_cpp)
        if not cpp_path.exists():
            results['errors'].append(f"Registration .cpp not found: {descriptor.registration_cpp}")
        else:
            content = cpp_path.read_text()
            new_content = transform_registration_cpp(
                content, descriptor.class_name, descriptor.tester_name,
                descriptor.namespace
            )
            if content != new_content:
                results['registration_cpp'] = 'transformed'
                if not dry_run:
                    cpp_path.write_text(new_content)
            else:
                results['registration_cpp'] = 'no_change'
    except Exception as e:
        results['errors'].append(f"Registration .cpp error: {e}")

    return results


def main():
    parser = argparse.ArgumentParser(
        description='Externalize embedded Test methods to TesterClass'
    )
    parser.add_argument(
        '--class', dest='class_name',
        help='Single class name to transform'
    )
    parser.add_argument(
        '--header',
        help='Path to header file (required with --class)'
    )
    parser.add_argument(
        '--test-header',
        help='Path to test header file (required with --class)'
    )
    parser.add_argument(
        '--cpp',
        help='Path to registration .cpp file (required with --class)'
    )
    parser.add_argument(
        '--struct', action='store_true',
        help='Use struct instead of class for TesterClass'
    )
    parser.add_argument(
        '--dry-run', action='store_true',
        help='Show what would be changed without modifying files'
    )
    parser.add_argument(
        '--phase', type=int,
        help='Phase number to process (requires descriptors built-in)'
    )

    args = parser.parse_args()

    descriptors = []

    # If --class specified, use command-line arguments
    if args.class_name:
        if not (args.header and args.test_header and args.cpp):
            print("Error: --header, --test-header, and --cpp are required with --class")
            sys.exit(1)
        descriptors.append(ClassDescriptor(
            class_name=args.class_name,
            tester_name=f'{args.class_name}Tester',
            header=args.header,
            test_header=args.test_header,
            registration_cpp=args.cpp,
            use_struct=args.struct
        ))
    elif args.phase:
        print(f"Phase {args.phase} not yet implemented in script.")
        sys.exit(1)
    else:
        print("Usage: python3 externalize_test.py --class <ClassName> --header <path> --test-header <path> --cpp <path> [--struct] [--dry-run]")
        print("Or:    python3 externalize_test.py --phase <N> [--dry-run]")
        sys.exit(1)

    # Apply transformations
    for descriptor in descriptors:
        results = apply_transformation(descriptor, args.dry_run)

        print(f"\n{descriptor.class_name}:")
        for key in ['header', 'test_header', 'registration_cpp']:
            if results[key]:
                print(f"  {key}: {results[key]}")

        if results['errors']:
            for error in results['errors']:
                print(f"  ERROR: {error}")

    if args.dry_run:
        print("\n[DRY RUN - no files modified]")


if __name__ == '__main__':
    main()
