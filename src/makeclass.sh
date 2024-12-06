#!/bin/bash

if [[ ${#@} -lt 2 ]]; then
  echo "Usage: $0 NewClassName include/TempLat/path/to/your/new/"
  echo "      "
  echo "      The class can be nested in namespace(s), by simply"
  echo "      writing Namespace::NewClassName."
  echo
  echo "      All classes will always be nested in namespace TempLat,"
  echo "      so adding a namespace will be in addition to TempLat."
  exit 0;
fi

userName="// File info: Main contributor(s):  `git config user.name`, Year: `date +%Y`"
licenseText=" This file is part of CosmoLattice, available at www.cosmolattice.net .
   Copyright Daniel G. Figueroa, Adrien Florio, Francisco Torrenti and Wessel Valkenburg.
   Released under the MIT license, see LICENSE.md."

fullClassName=$1
className=`echo $fullClassName | sed 's/.*::\([^:]*\)$/\1/g'`
#namespaces=`echo $fullClassName | sed 's/\(.*::\)\([^:]*\)$/\1/g'`
namespaces=""
namespacesSplit=`echo $namespaces | sed 's/::/ /g'`

namespaceTabs=""
namespaceHead=""
namespaceTail=""

for ns in $namespacesSplit; do
inserttabs="    ${inserttabs}"
namespaceHead=`printf '%s\n\n%s\n\n' "$namespaceHead" "${inserttabs}namespace $ns {"`
namespaceTail=`printf '%s\n\n%s\n\n' "${inserttabs}} /* $ns */" "$namespaceTail"`
done


fullPath=`echo "$2" | sed -e 's,/*$,,g' -e 's,^\./,,g'`
lowerCase=`echo ${className} | awk '{print tolower($0)}'`
fName=` echo ${lowerCase} | sed -e 's/\./_/g' -e 's,\/,_,g'`".h"
relPathToFile=`echo "${fullPath}/${fName}" | sed 's,include/,,g'`
FNAME=`echo ${relPathToFile} | awk '{print toupper($0)}' | sed -e 's/\./_/g' -e 's,\/,_,g'`
testname=`echo $relPathToFile | sed 's/\.h$/_test.h/g'`
TESTNAME=`echo ${testname} | awk '{print toupper($0)}' | sed -e 's/\./_/g' -e 's/-/_/g' -e 's,\/,_,g'`
cppFName=tests/`echo ${relPathToFile} | sed 's,\([^/]*\)\.h,\1.cpp,g'`
dname=`dirname "${cppFName}"`


echo "Class name:         $className"
echo "path:               $fullPath"
echo "relative path:      $relPathToFile"
echo "header file:        $fName"
echo "#include guard tag: $FNAME"
echo "test header file:   $testname"
echo "#include guard tag: $TESTNAME"
echo "CPP file for test:  ${cppFName}"
echo
echo

if [[ -f "include/${relPathToFile}" ]]; then
  echo "ERROR: file \"${1}\" exists. Please remove it manually before proceeding."
  exit 1;
fi


mkdir -p "${fullPath}"
mkdir -p "${dname}"


(echo "#ifndef $FNAME"
echo "#define $FNAME"
echo ""
#echo "/* ${userName} */"
echo "/* ${licenseText} */"
echo
echo "${userName}"
echo
echo "#include \"TempLat/util/tdd/tdd.h\""
echo
echo "namespace TempLat {"
printf '%s\n' "$namespaceHead"
echo
echo "${inserttabs}    /** \brief A class which"
echo "${inserttabs}     * \todo Write this. "
echo "${inserttabs}     * "
echo "${inserttabs}     * "
echo "${inserttabs}     * Unit test: make test-${lowerCase}"
echo "${inserttabs}     **/"
echo
echo "${inserttabs}    class ${className} {"
echo "${inserttabs}    public:"
echo "${inserttabs}        /* Put public methods here. These should change very little over time. */"
echo "${inserttabs}        ${className}() {"
echo
echo "${inserttabs}        }"
echo
echo
echo "${inserttabs}    private:"
echo "${inserttabs}        /* Put all member variables and private methods here. These may change arbitrarily. */"
echo
echo
echo
echo "${inserttabs}    public:"
echo "${inserttabs}        static inline void Test(TDDAssertion& tdd);"
echo "${inserttabs}    };"
echo
printf '%s\n\n' "$namespaceTail"
echo "} /* TempLat */"
echo
echo "#include \"$testname\""
echo
echo
echo "#endif") > "include/${relPathToFile}"

git add "include/${relPathToFile}"

(echo "#ifndef $TESTNAME"
echo "#define $TESTNAME"
echo ""
#echo "/* ${userName} */"
echo "/* ${licenseText} */"
echo
echo "${userName}"
echo
echo "inline void TempLat::${namespaces}${className}::Test(TempLat::TDDAssertion& tdd) {"
echo
echo "    /* Default is to fail: to remind yourself to implement something here. */"
echo "    tdd.verify( false );"
echo
echo "}"
echo
echo "#endif") > "include/${testname}"

git add "include/${testname}"

# create the implementation file

if [[ -f "${ofname}" ]]; then
  echo "ERROR: file \"${ofname}\" exists. Please remove it manually before proceeding."
  exit 1;
fi

(
echo ""
#echo "/* ${userName} */"
echo "/* ${licenseText} */"
echo
echo "${userName}"
echo "#include \"$relPathToFile\""
echo ""
echo "namespace {"
echo "    TempLat::TDDContainer<TempLat::${namespaces}${className}> test;"
echo "}"
echo ""
) > "${cppFName}"

git add "${cppFName}"
