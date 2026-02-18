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


def transform_header(content: str, class_name: str, tester_name: str,
                     use_struct: bool = False, namespace: str = "TempLat") -> str:
    """
    Transform header file:
    1. Remove embedded #ifdef TEMPLATTEST block from class body
    2. Add TesterClass before namespace closing brace
    """

    # Step 1: Remove embedded block from inside the class
    # Pattern: \n#ifdef TEMPLATTEST\n  public:\n    static inline void Test(TDDAssertion &tdd);\n#endif\n
    # Allow for whitespace/indentation variations
    embedded_pattern = (
        r'\n\s*#ifdef TEMPLATTEST\s*\n\s*public:\s*\n\s*'
        r'static inline void Test\(TDDAssertion &tdd\);\s*\n\s*#endif\s*\n'
    )
    content = re.sub(embedded_pattern, '\n', content, count=1)

    # Also handle alternative pattern where public: is already present
    # Try simpler pattern if above didn't work
    if '#ifdef TEMPLATTEST' in content:
        # More flexible: match any #ifdef TEMPLATTEST ... static inline void Test ... #endif inside the class
        simple_pattern = (
            r'#ifdef TEMPLATTEST\s*\n\s*public:\s*\n\s*'
            r'static inline void Test\(TDDAssertion &tdd\);\s*\n\s*#endif'
        )
        content = re.sub(simple_pattern, '', content, count=1)

    # Step 2: Add TesterClass before namespace closing
    # Find the line with "} // namespace <namespace>"
    namespace_close = f'}} // namespace {namespace}'

    if namespace_close in content:
        tester_class_type = 'struct' if use_struct else 'class'
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
                          namespace: str = "TempLat") -> str:
    """
    Transform test header file:
    Change function qualifier from Foo::Test to FooTester::Test
    """
    old_qualifier = f'{namespace}::{class_name}::Test('
    new_qualifier = f'{namespace}::{tester_name}::Test('
    content = content.replace(old_qualifier, new_qualifier, 1)
    return content


def transform_registration_cpp(content: str, class_name: str, tester_name: str,
                               namespace: str = "TempLat") -> str:
    """
    Transform registration .cpp file:
    Change TDDContainer<Foo> to TDDContainer<FooTester>
    """
    old_container = f'TDDContainer<{namespace}::{class_name}>'
    new_container = f'TDDContainer<{namespace}::{tester_name}>'
    content = content.replace(old_container, new_container, 1)
    return content


def apply_transformation(descriptor: ClassDescriptor, dry_run: bool = False) -> dict:
    """
    Apply transformation to all three files for a given class descriptor.
    Returns a dict with status for each file.
    """
    results = {
        'class': descriptor.class_name,
        'header': None,
        'test_header': None,
        'registration_cpp': None,
        'errors': []
    }

    # Transform header
    try:
        header_path = Path(descriptor.header)
        if not header_path.exists():
            results['errors'].append(f"Header not found: {descriptor.header}")
        else:
            content = header_path.read_text()
            new_content = transform_header(
                content, descriptor.class_name, descriptor.tester_name,
                descriptor.use_struct, descriptor.namespace
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
                descriptor.namespace
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
