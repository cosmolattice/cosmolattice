#! /usr/bin/env python3
#		convertbiblio: a script for converting standard
#		bibliography to the database format for BibTex/LaTex
#		use this script as a filter, e.g.:
#		convertbiblio listOfBibitemRefs.tex > somefilename.bib
#		and then replace the references in latex with the 2 lines:
#		\bibliographystyle{unsrt} % or prsty or something else
#		\bibliography{somefilename} % the output of convertbiblio
#		run latex / bibtex / latex / latex ... et voila!
#
#	     (initially written in shell/sed/awk, then in perl, now in python)
version = 4.01  #	               by  Nick  -  20/07/2025
import sys
import re

def usage(commandname):
    print(commandname,'''[options] filename1 [filename2 ...]
convert list(s) of \\bibitem data into bibtex data (to stdout)
options are:
    -h / --help            print this usage summary
    -d                     debug - print extra debugging messages
''')

def fromcommastoand(nlist,key): #put a list of names in the bibtex format
    if len(nlist)<1:
        result=""
        print("convertbiblio WARNING:",key,"lacks authors: misformatted quote?", file=sys.stderr)
    else:
        result=re.sub(r'^\s*','',nlist[0])
        for aut in nlist[1:len(nlist)]:
            if re.search(r'\\textit\s*\{|\{\s*\\it\s+|\{\s*\\sl\s+',aut): break
            result+=" and "+aut
    return result

def process(totline,titledata):
# preprocessing:
    totline=re.sub(r'\s+',' ',totline)
# the following 2 lines change e.g. M.A. Smith to M.\ A.\ Smith.  They can cause trouble
#    totline=re.sub(r'([A-Zh])\.\s*([{A-Z])\.\s*([{A-Z])',r'\1.\\ \2.\\ \3',totline)
#    totline=re.sub(r'([A-Zh])\.\s*([{A-Z])',r'\1.\\ \2',totline)
    totline=re.sub(r'\);.+',r').',totline) # annihilate multiple refs with ; !!

# storeup&removal of commented notes, in [ ]
    if re.search(r' \[.+\]',totline):
        comstuff=re.sub(r'.*( \[.+\]).*',r'\1',totline)
        totline =re.sub(r'( \[.+\])','',totline)
        if debug>0: print("%COMSTUFF 	:",comstuff)
    else:
        comstuff=''

    if debug>0: print("%PROCESSED LINE  :",totline)

# reference recognition and parsing
    lista=re.split(r', and | and |, ',totline)
    nfields=len(lista)
    key=re.sub(r'\\bibitem\{([^\}]*)\}.*',r'\1',lista[0])
    if(len(key)==0):
        print("convertbiblio ERROR  : no key from ",totline, file=sys.stderr)
    totline=re.sub(r'\\bibitem\{[^\}]*\}','',totline)
    originalline=totline
    lista[0]=re.sub(r'\\bibitem\{[^\}]*\}','',lista[0])
    firstauthor=re.sub(r'^\s*','',lista[0])
    if debug>0:
        print("%NFIELDS  	:",nfields)
        print("%KEY      	:",key)
        print("%FIRST AUTHOR	:",firstauthor)

    if len(totline) > 6: # line must have something inside, or else drop it!

        journal=""
        volume=0
        page=0
        year=0
        text=""
        authorlist=""
        title=""
        booktitle=""
        editor=""
        publisher=""
        note=""
        url=""
        type=""

        url=""
        if re.search(r'\\url\{',totline):
            url    =re.sub(r'(.*)\\url\{([^\}]*)\}[,.]?(.*)',r'\2',totline)
            totline=re.sub(r'(.*)\\url\{([^\}]*)\}[,.]?(.*)',r'\1 \3',totline)
            if debug>0:
                print("%URL     	:",url)
        
        jouandvol=lista[nfields-2]
        if re.search(r' edited by | ed\.s? by ',totline):
            type="inbook"  # the only reasonable record for in-volume contribs
            ibsplit=re.split(r' edited by | ed\.s? by ',totline)

            if re.search(r'^\s*(\\textit\s*\{|\{\s*\\it\s+|\{\s*\\sl\s+)',ibsplit[0]):
                type="book" # reference to whole volume, no authors
                authorlist=""
            else:
                authorlist=fromcommastoand(lista,key)
            divided=re.split(r',\s+in\s+',ibsplit[0])
#            print "QUQU",divided
            booktitle=ibsplit[0]
            if len(divided)>1:
                tit=re.split(r'\\textit\s*\{|\{\s*\\it\s+|\{\s*\\sl\s+|\},'," "+divided[0])
#                print "QUIX", tit
                if len(tit)>1:
                    title=tit[1]
                    title=re.sub(r'\}$','',title)
                if(len(tit)>2):
                    note=tit[2]
                booktitle=divided[1]

            btit=re.split(r'\\textit\s*\{|\{\s*\\it\s*|\{\s*\\sl\s*|\},'," "+booktitle)
#            print "QUIY", btit
            if len(btit)>1:
                booktitle=btit[1]
            if(len(btit)>2):
                note=note+btit[2]
                note=re.sub(r',$','',note)

            if type=="book":
                title=booktitle

            editorpublisherpage=re.split(r',? \(|\)[.,]? ',ibsplit[1])
            if len(editorpublisherpage)>2:
#                if len(authorlist)==0:
                editor=editorpublisherpage[0]
                edlist=re.split(r', and | and |, ',editor)
                editor=fromcommastoand(edlist,key)
#                else:
#                    text="edited by "+editorpublisherpage[0]
                ytest=re.sub(r'.*\s(\d{4})',r'\1',editorpublisherpage[1])
                publisher=re.sub(r'(.*),\s+\d{4}',r'\1',editorpublisherpage[1])
                page=re.sub(r'\s*p\.\s*|\s*pag[e.]?\s*',r'',editorpublisherpage[2])
                page=re.sub(r'^\s*\\\s*','',page)
                page=re.sub(r'\.\s*$','',page)
#                print "QUAAA",editor,publisher,page,ytest
            else:
                ytest=re.sub(r'.*\s(\d{4})\).?\s*$',r'\1',ibsplit[1])
            if debug>0:
                print("%TYPE     	:",type)
                print("%TITLE     	:",title)
                print("%BOOKTITLE     	:",booktitle)
                print("%YTEST      	:",ytest)
                print("%PAGE     	:",page)
                print("%EDITOR     	:",editor)
                print("%TEXT     	:",text)
                print("%PUBLISHER   	:",publisher)
            if len(ytest)==4 and int(ytest)-1000.>0:
                year=int(ytest)
        elif re.search(r'\\textbf\s*\{|\{\s*\\bf ',jouandvol):
            type="article"
            title=" "+titledata # if available...
            if debug>0:
                print("%TYPE     	:",type)

            authorlist=fromcommastoand(lista[0:-2],key)
                
            journal=re.sub(r'\s*(.*)\s*(\\textbf\s*\{|\{\s*\\bf )[0-9-]*\}.*',r'\1',jouandvol)
            journal=re.sub(r'\\\s*$',r'',journal)
            volume=re.sub(r'.*(\\textbf\s*\{|\{\s*\\bf )([0-9-]*)\}.*',r'\2',jouandvol)
#            page=re.sub(r'\s*(\w*).*',r'\1',lista[nfields-1])
            page=re.sub(r'\s.*','',lista[nfields-1])

            ytest=re.sub(r'.*\((\d*)\).*',r'\1',lista[nfields-1])
            if debug>0:
                print("%YTEST      	:",ytest)
            if len(ytest)==4 and re.search(r'\d{4}',ytest) and int(ytest)-1000.>0:
                year=int(ytest)

            if debug>0:
                print("%AUTHORLIST	:",authorlist)
                print("%JOURNALANDVOL 	:",jouandvol)
                print("%JOURNAL   	:",journal)
                print("%VOLUME    	:",volume)
                print("%PAGE     	:",page)
        elif re.search(r'submitted to ',totline): # submitted paper
            type="article"
            title=" " # unknown title
            if debug>0:
                print("%TYPE     	:",type)

            authorlist=fromcommastoand(lista[0:-2],key)

            journal=re.sub(r'.*submitted to (.*) \(.*',r'\1',totline)
#            journal=re.sub(r'\\\s*$',r'',journal)
            volume=" "
            page=" "
            ytest=re.sub(r'.*\((\d*)\).*',r'\1',lista[nfields-1])
            comstuff="submitted"
            if debug>0:
                print("%YTEST      	:",ytest)
            if len(ytest)==4 and re.search(r'\d{4}',ytest) and int(ytest)-1000.>0:
                year=int(ytest)

            if debug>0:
                print("%AUTHORLIST	:",authorlist)
                print("%JOURNALANDVOL 	:",jouandvol)
                print("%JOURNAL   	:",journal)
                print("%VOLUME    	:",volume)
                print("%PAGE     	:",page)
                print("%NOTE     	:",note)
        else: # book or note
            title=""
            ititle=0
            for test in lista:
                if re.search(r'\\textit\s*\{|\{\s*\\it |\{\s*\\sl ',test):
                    break
                ititle=ititle+1

            if ititle<nfields:
                authorlist=firstauthor
                for i in range(1,ititle):
                    authorlist+=" and "+lista[i]

                booksplit=re.split(r'\\textit\s*\{|\{\s*\\it |\{\s*\\sl ',totline)
#                print "QUIII",booksplit
                titleandpublisher=booksplit[1]
                booksplit=re.split(r'\},?\s|\s\(',titleandpublisher)
                if len(booksplit)>2:
                    note=booksplit[1]
                booksplit=re.split(r'\}[^\}]*\s\(',titleandpublisher)                
#                print "QUXXX",booksplit
#                if len(booksplit) == 1:
#                    booksplit=re.split(r' \(',titleandpublisher)
                if len(booksplit) == 1:
                    type="misc"
                    text=titleandpublisher
                    authorlist=firstauthor
                else:
                    title=booksplit[0]
#                    print "QUOOO",booksplit[0],"xxxxx",booksplit[1]
                    publisherandyear=booksplit[1]
                    eysplit=re.split(r' \(|\).\s*',publisherandyear)
#                    print "QUAAA",eysplit
                    publisher=eysplit[0]
                    if len(eysplit)>=2:
                        page=re.sub(r'\s+$',r'',eysplit[1])
                    ytest =re.sub(r'.*\s(\d{4})$',r'\1',publisher)
                    publisher=re.sub(r'(.*),\s?\d{4}$',r'\1',publisher)
                    if len(ytest)==4:
                        type="book"
                        year=int(ytest)
                    else:
                        type="misc"
                        authorlist=firstauthor
                        text=re.sub(r'\s*(.*).?\s*$',r'\1',totline)
                    if debug>0:
                        print("%TYPE     	:",type)
                        print("%AUTHORLIST	:",authorlist)
                        print("%TITLE    	:",title)
                        print("%PUBLISHER   	:",publisher)
                        print("%PAGE      	:",page)
                        print("%YTEST      	:",ytest)
            else: # nonmatching stuff: use misc
#                print "QUOOO",ititle,nfields
                type="misc"
                ytest=re.sub(r'.*\s\((\d{4})\).?\s*$',r'\1',lista[nfields-1])
                if debug>0:
                    print("%YTEST      	:",ytest)
                if len(ytest)==4 and re.search(r'\d{4}',ytest) and int(ytest)-1000.>0:
                    year=int(ytest)
                    if(re.search(r'\d\s+\('+ytest,lista[nfields-1])):
                        print("convertbiblio WARNING:",key,"might be a misformatted article (using misc)", file=sys.stderr)

                text=re.sub(r'\s*(.*).?\s*$',r'\1',totline)
                authorlist=firstauthor
                if debug>0:
                    print("%TYPE     	:",type)
                    print("%TEXT     	:",text)
# neither article nor book:
#            type="misc"
#        if debug>0:
#            print "%TYPE    	:",type
#            print "%YEAR    	:",year

        authorlist=re.sub(r' and and ',r' and ',authorlist)
        if type=="misc":
#            print "QUII",authorlist # THIS CODE IS STILL BUGGY:
            authorlist=re.sub(r'(|[A-Z][a-z]?\.\\?\s[A-Z][a-z]+,?)+.*',r'\1',authorlist)
#            print "QUAA",authorlist

        if type=="book" and re.search(r'\W[tT]hesis\W',totline):
            if re.search(r'\W(PhD|Ph\.D\.)\W',totline):
                type="phdthesis"
            else:
                type="mastersthesis"

        print("@"+type+"{"+key+",")
        if len(authorlist) > 0:
            print("author = {"+authorlist+"},")
        if year > 0 and type!="misc":
            print("year = {",year,"},")
        elif type!="misc": # for misc no year is to be hoped for
            print("convertbiblio WARNING: no year in ",key, file=sys.stderr)

        if type=="inbook":
            if len(editor)>0:
                print("editor = {"+editor+"},")
            print("publisher = { "+publisher+" },")
            if title != "":
                print("title = {"+title+"},")
            print("booktitle = {"+booktitle+"},")
            print("pages = {",page,"},")
            if len(text)+len(note)>0:
                print("note = {"+text+comstuff+note+"},")
        elif type=="article":
            print("title = {"+title+"},")
            if len(journal)>0:
                print("journal = {"+journal+"},")
            else:
                print("convertbiblio WARNING: no journal in ",key," (maybe a wrong comma?)", file=sys.stderr)
            if len(volume)>0:
                print("volume = {"+volume+"},")
            else:
                print("convertbiblio WARNING: no volume in ",key, file=sys.stderr)
            if len(page)>0:
                print("pages = {",page,"},")
            else:
                print("convertbiblio WARNING: no pages in ",key, file=sys.stderr)
            if len(comstuff)>0:
                print("note = {"+comstuff+"},")
        elif type=="book" or type=="phdthesis" or type=="mastersthesis":
            print("title = {"+title+"},")
            if type=="phdthesis" or type=="mastersthesis":
                print("school = { "+publisher+" },")
            else:
                print("publisher = { "+publisher+" },")
            if len(editor)>0:
                print("editor = {"+editor+"},")
            if len(page)>0:
                print("pages = {"+page+"},")
            if len(note)>0:
                print("note = {"+note+"},")
        elif type=="misc":
#            print "note = {"+text+comstuff+"},"
            print("note = {"+originalline+"},")
        if len(url)>0 and type!="misc": # trying to extract url in misc is calling for trouble
            print("url = {"+url+"},")
        print("}\n")


def main(argv):
    import re
    import string
    import os
    import getopt    

    commandname=os.path.basename(sys._getframe().f_code.co_filename)
    global debug
    debug = 0
    filenames = []
    try:
        opts, filenames = getopt.getopt(argv, "hg:d", ["help","d"])
    except getopt.GetoptError:
        usage(commandname)
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage(commandname)
            sys.exit()               
        elif opt == '-d':
            debug = 1

    if len(filenames)==0:
        filenames.append("-")

    progname=re.sub(r'.*\/','',commandname)
    print('''% This is an automatically generated bibligraphy file.
%	DO NOT EDIT!!	DO NOT EDIT!!	DO NOT EDIT!!	DO NOT EDIT!!
% In case corrections are needed 
% --->  edit the original list of references, and then
% re-generate the present file by running''',progname,'''
% [freely available at http://materia.fisica.unimi.it/manini/scripts.html].
%
% Converted file(s):''',filenames,'''
% Conversion script''',progname,'''[version''',version,''']
''')
    for filen in filenames:
        if filen=="-":
            f=sys.stdin
        else:
            f = open(filen, 'r')
# put a whole reference in one single line:
        totline=''
        titledata='' # we store here comments that come before the proper reference, not those after it
        keepcomm=True
        for line in f:
#            print line,
#            line=re.sub(r'([^\\])%.*',r'\1',line)  # remove latex comments
#            line=re.sub(r'^\%.*','',line)          # remove latex commented lines

            try:
                line,comment=re.split('%+',line,maxsplit=1)
            except ValueError:
                comment=''
#            print line,"COCO", comment
            if re.search(r'\\bibitem',line):
               process(totline,titledata)
               totline=''
               titledata=''
               keepcomm=True
            if re.search(r'\\bf|\\mathbf',line):
               keepcomm=False
            totline+=re.sub(r'\n',' ',line)
            if keepcomm: # only comments before the actual reference
                titledata+=re.sub(r'\n',' ',comment)
        process(totline,titledata)
    pass


if __name__ == "__main__":
    main(sys.argv[1:])
