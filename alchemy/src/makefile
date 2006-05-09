INLINE_LIMIT=-finline-limit=100000
#INLINE_WARN=-Winline

######### Profile
#CLFLAGS=-pg -g -static -DNDEBUG $(INLINE_WARN) $(INLINE_LIMIT) 

######### Profile & optimize
#CLFLAGS=-pg -g -O3 -static -DNDEBUG $(INLINE_WARN) $(INLINE_LIMIT) 

######## Optimize
#CLFLAGS=-O3 -static $(INLINE_WARN) $(INLINE_LIMIT)

######## Optimize and turn off asserts
CLFLAGS=-O3 -static -DNDEBUG $(INLINE_WARN) $(INLINE_LIMIT)

######### Debug
#CLFLAGS=-g



#Add the path to the alchemy dir here e.g. /home/alchemy
#This is not a relative path because the path name is
#used to set the location of the maxwalksat executable
#in src/infer/maxwalksat.cpp.
BASEDIR=

GPP=g++
FLEX=flex
BISON=bison

BIN=$(BASEDIR)/bin
OBJDIR=$(BASEDIR)/bin/obj
MF=makefile

SRCDIR=$(BASEDIR)/src
UTIL=$(SRCDIR)/util
PARSER=$(SRCDIR)/parser
LOGIC=$(SRCDIR)/logic
INFER=$(SRCDIR)/infer
LEARNWTS=$(SRCDIR)/learnwts
LEARNSTRUCT=$(SRCDIR)/learnstruct

REPLACEFOLCPP=$(PARSER)/replacefolcpp.pl
MWSLOC=$(INFER)/mwsloc.pl
MAXWALKSATDIR=$(INFER)/Maxwalksat20

LIBCINC= -I. -I$(UTIL) -I$(LOGIC) -I$(PARSER) -I$(LEARNWTS) -I$(LEARNSTRUCT) \
	-I$(INFER)
LIBLINC= -L. -L$(UTIL) -L$(LOGIC) -L$(PARSER) -L$(LEARNWTS) -L$(LEARNSTRUCT) \
	-L$(INFER)
LIBLINK= -ldl

CFLAGS=$(CLFLAGS) -Wall $(BIGNUM) $(LIBCINC)
LFLAGS=$(CLFLAGS) -Wall $(BIGNUM) $(LIBLINC)


#############################################################################
#add non-executables .cpp files to OBJS 
#if .cpp file reside in a dir other than the current one, add .o rule below
OBJS=$(OBJDIR)/arguments.o $(OBJDIR)/powerset.o \
	$(OBJDIR)/fol.o $(OBJDIR)/listobj.o \
	$(OBJDIR)/predicatetemplate.o $(OBJDIR)/functiontemplate.o \
	$(OBJDIR)/function.o $(OBJDIR)/predicate.o $(OBJDIR)/term.o \
	$(OBJDIR)/domain.o $(OBJDIR)/clause.o $(OBJDIR)/clausesampler.o \
	$(OBJDIR)/groundclause.o $(OBJDIR)/intclause.o $(OBJDIR)/maxwalksat.o $(OBJDIR)/lazywalksat.o \
	$(OBJDIR)/structlearn.o

#add .cpp files (including executables') to CPPFILES.
CPPFILES=$(UTIL)/arguments.cpp $(UTIL)/powerset.cpp \
	$(PARSER)/fol.cpp $(PARSER)/listobj.cpp \
	$(LOGIC)/predicatetemplate.cpp $(LOGIC)/functiontemplate.cpp \
	$(LOGIC)/function.cpp $(LOGIC)/predicate.cpp $(LOGIC)/term.cpp \
	$(LOGIC)/domain.cpp  $(LOGIC)/clause.cpp $(LOGIC)/clausesampler.cpp\
	$(INFER)/groundclause.cpp $(INFER)/intclause.cpp $(INFER)/maxwalksat.cpp $(INFER)/lazywalksat.cpp \
	$(LEARNSTRUCT)/structlearn.cpp \
	$(LEARNWTS)/learnwts.cpp $(INFER)/infer.cpp $(LEARNSTRUCT)/learnstruct.cpp 


##############################################################################
#EXECUTABLES: add executables rules here

all: learnwts infer learnstruct

learnwts: $(OBJDIR)/learnwts.oo
	cp $(OBJDIR)/learnwts.oo $(BIN)/learnwts

infer: $(OBJDIR)/infer.oo
	cp $(OBJDIR)/infer.oo $(BIN)/infer

learnstruct: $(OBJDIR)/learnstruct.oo
	cp $(OBJDIR)/learnstruct.oo $(BIN)/learnstruct


$(OBJDIR)/learnwts.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/learnwts.o -c $(LEARNWTS)/learnwts.cpp

$(OBJDIR)/infer.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/infer.o -c $(INFER)/infer.cpp

$(OBJDIR)/learnstruct.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/learnstruct.o -c $(LEARNSTRUCT)/learnstruct.cpp


%.oo: %.o $(OBJS) $(LIBS)
	echo $<
	$(GPP) $(LFLAGS) -o $@ $(OBJS) $< $(LIBLINK)


##############################################################################
#add add non-executables .o rules here

$(OBJDIR)/fol.o: $(PARSER)/fol.y $(PARSER)/follex.y
	$(FLEX) -o$(PARSER)/follex.cpp $(PARSER)/follex.y 
	$(BISON) $(PARSER)/fol.y;
	mv -f fol.tab.c  $(PARSER)/fol.cpp
	$(REPLACEFOLCPP) $(PARSER)/fol.cpp
	$(GPP) $(CFLAGS) -o $(OBJDIR)/fol.o -c $(PARSER)/fol.cpp


#.o rule
#$(OBJDIR)/file.o:
#	$(GPP) $(CFLAGS) -o $(OBJDIR)/file.o -c dir/file.cpp


$(OBJDIR)/arguments.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/arguments.o -c $(UTIL)/arguments.cpp

$(OBJDIR)/powerset.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/powerset.o -c $(UTIL)/powerset.cpp

$(OBJDIR)/listobj.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/listobj.o -c $(PARSER)/listobj.cpp 

$(OBJDIR)/predicatetemplate.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/predicatetemplate.o -c $(LOGIC)/predicatetemplate.cpp 

$(OBJDIR)/functiontemplate.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/functiontemplate.o -c $(LOGIC)/functiontemplate.cpp 

$(OBJDIR)/function.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/function.o -c $(LOGIC)/function.cpp 

$(OBJDIR)/predicate.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/predicate.o -c $(LOGIC)/predicate.cpp 

$(OBJDIR)/term.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/term.o -c $(LOGIC)/term.cpp

$(OBJDIR)/domain.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/domain.o -c $(LOGIC)/domain.cpp  

$(OBJDIR)/clause.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/clause.o -c $(LOGIC)/clause.cpp  

$(OBJDIR)/clausesampler.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/clausesampler.o -c $(LOGIC)/clausesampler.cpp  

$(OBJDIR)/groundclause.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/groundclause.o -c $(INFER)/groundclause.cpp

$(OBJDIR)/intclause.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/intclause.o -c $(INFER)/intclause.cpp

$(OBJDIR)/structlearn.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/structlearn.o -c $(LEARNSTRUCT)/structlearn.cpp

$(OBJDIR)/lazywalksat.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/lazywalksat.o -c $(INFER)/lazywalksat.cpp

$(OBJDIR)/maxwalksat.o:
	gcc -O2 $(MAXWALKSATDIR)/maxwalksat.c -lm -o $(MAXWALKSATDIR)/maxwalksat
	$(MWSLOC) $(INFER) $(MAXWALKSATDIR)
	$(GPP) $(CFLAGS) -o $(OBJDIR)/maxwalksat.o -c $(INFER)/maxwalksat.cpp



###############################################################################


clean:
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.oo

depend:
	@echo 'updating the dependencies for:'
	@echo '    ' $(CPPFILES)
	@( \
	< $(MF) sed -n '1,/^###.*SUDDEN DEATH/p'; \
	    echo '#' ; \
	    echo '# dependencies generated on: ' `date` ; \
	    echo '#' ; \
	    for i in $(CPPFILES); do \
	        echo -n "$(OBJDIR)/" ; \
	        $(GPP) -MM $(CFLAGS) $$i ; \
	        echo; \
	    done \
	) > $(MF).new
	@mv $(MF) $(MF).last
	@mv $(MF).new $(MF)


############ Everything below here is subject to SUDDEN DEATH #####
#
# dependencies generated on:  Mon May 8 15:58:22 PDT 2006
#
/homes/gws/marcs/release/alchemy/bin/obj/arguments.o: /homes/gws/marcs/release/alchemy/src/util/arguments.cpp \
  /homes/gws/marcs/release/alchemy/src/util/arguments.h

/homes/gws/marcs/release/alchemy/bin/obj/powerset.o: /homes/gws/marcs/release/alchemy/src/util/powerset.cpp \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h

/homes/gws/marcs/release/alchemy/bin/obj/fol.o: /homes/gws/marcs/release/alchemy/src/parser/fol.cpp \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/parser/follex.cpp \
  /homes/gws/marcs/release/alchemy/src/parser/folhelper.h \
  /homes/gws/marcs/release/alchemy/src/parser/strfifolist.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/parser/listobj.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/parser/internals.h

/homes/gws/marcs/release/alchemy/bin/obj/listobj.o: /homes/gws/marcs/release/alchemy/src/parser/listobj.cpp \
  /homes/gws/marcs/release/alchemy/src/parser/listobj.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h

/homes/gws/marcs/release/alchemy/bin/obj/predicatetemplate.o:  \
 /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h

/homes/gws/marcs/release/alchemy/bin/obj/functiontemplate.o:  \
 /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h

/homes/gws/marcs/release/alchemy/bin/obj/function.o: /homes/gws/marcs/release/alchemy/src/logic/function.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h

/homes/gws/marcs/release/alchemy/bin/obj/predicate.o: /homes/gws/marcs/release/alchemy/src/logic/predicate.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h

/homes/gws/marcs/release/alchemy/bin/obj/term.o: /homes/gws/marcs/release/alchemy/src/logic/term.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h

/homes/gws/marcs/release/alchemy/bin/obj/domain.o: /homes/gws/marcs/release/alchemy/src/logic/domain.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h

/homes/gws/marcs/release/alchemy/bin/obj/clause.o: /homes/gws/marcs/release/alchemy/src/logic/clause.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/infer/mrf.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/freestoremanager.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/infer/wsutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksatparams.h \
  /homes/gws/marcs/release/alchemy/src/infer/convergencetest.h \
  /homes/gws/marcs/release/alchemy/src/util/meanvariance.h \
  /homes/gws/marcs/release/alchemy/src/infer/gelmanconvergencetest.h

/homes/gws/marcs/release/alchemy/bin/obj/clausesampler.o:  \
 /homes/gws/marcs/release/alchemy/src/logic/clausesampler.cpp \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h

/homes/gws/marcs/release/alchemy/bin/obj/groundclause.o:  \
 /homes/gws/marcs/release/alchemy/src/infer/groundclause.cpp \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h

/homes/gws/marcs/release/alchemy/bin/obj/intclause.o: /homes/gws/marcs/release/alchemy/src/infer/intclause.cpp \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h

/homes/gws/marcs/release/alchemy/bin/obj/maxwalksat.o: /homes/gws/marcs/release/alchemy/src/infer/maxwalksat.cpp \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksat.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/freestoremanager.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/infer/wsutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksatparams.h

/homes/gws/marcs/release/alchemy/bin/obj/lazywalksat.o:  \
 /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.cpp \
  /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/freestoremanager.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/infer/wsutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksatparams.h

/homes/gws/marcs/release/alchemy/bin/obj/structlearn.o:  \
 /homes/gws/marcs/release/alchemy/src/learnstruct/structlearn.cpp \
  /homes/gws/marcs/release/alchemy/src/learnstruct/structlearn.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/pseudologlikelihood.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/indextranslator.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/lbfgsb.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/pseudologlikelihood.h

/homes/gws/marcs/release/alchemy/bin/obj/learnwts.o: /homes/gws/marcs/release/alchemy/src/learnwts/learnwts.cpp \
  /homes/gws/marcs/release/alchemy/src/util/arguments.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/pseudologlikelihood.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/indextranslator.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/lbfgsb.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/votedperceptron.h \
  /homes/gws/marcs/release/alchemy/src/infer/infer.h \
  /homes/gws/marcs/release/alchemy/src/util/util.h \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/infer/mrf.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/freestoremanager.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/infer/wsutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksatparams.h \
  /homes/gws/marcs/release/alchemy/src/infer/convergencetest.h \
  /homes/gws/marcs/release/alchemy/src/util/meanvariance.h \
  /homes/gws/marcs/release/alchemy/src/infer/gelmanconvergencetest.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/learnwts.h \
  /homes/gws/marcs/release/alchemy/src/util/util.h \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h

/homes/gws/marcs/release/alchemy/bin/obj/infer.o: /homes/gws/marcs/release/alchemy/src/infer/infer.cpp \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/infer/mrf.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/freestoremanager.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/infer/wsutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksatparams.h \
  /homes/gws/marcs/release/alchemy/src/infer/convergencetest.h \
  /homes/gws/marcs/release/alchemy/src/util/meanvariance.h \
  /homes/gws/marcs/release/alchemy/src/infer/gelmanconvergencetest.h \
  /homes/gws/marcs/release/alchemy/src/util/arguments.h \
  /homes/gws/marcs/release/alchemy/src/util/util.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/learnwts.h \
  /homes/gws/marcs/release/alchemy/src/util/util.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/indextranslator.h \
  /homes/gws/marcs/release/alchemy/src/infer/infer.h

/homes/gws/marcs/release/alchemy/bin/obj/learnstruct.o:  \
 /homes/gws/marcs/release/alchemy/src/learnstruct/learnstruct.cpp \
  /homes/gws/marcs/release/alchemy/src/util/arguments.h \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/dualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/equalstr.h \
  /homes/gws/marcs/release/alchemy/src/util/constdualmap.h \
  /homes/gws/marcs/release/alchemy/src/util/strint.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicatetemplate.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/logic/functiontemplate.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/term.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/logic/function.h \
  /homes/gws/marcs/release/alchemy/src/util/hashstring.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashlist.h \
  /homes/gws/marcs/release/alchemy/src/util/arraysaccessor.h \
  /homes/gws/marcs/release/alchemy/src/util/powerset.h \
  /homes/gws/marcs/release/alchemy/src/util/multdarray.h \
  /homes/gws/marcs/release/alchemy/src/util/hashint.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/logic/groundpreds.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/predicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundclause.h \
  /homes/gws/marcs/release/alchemy/src/util/hasharray.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/hash.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausesampler.h \
  /homes/gws/marcs/release/alchemy/src/logic/truefalsegroundingsstore.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausehelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/mlnhelper.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/database.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/learnwts.h \
  /homes/gws/marcs/release/alchemy/src/util/util.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/indextranslator.h \
  /homes/gws/marcs/release/alchemy/src/infer/infer.h \
  /homes/gws/marcs/release/alchemy/src/util/util.h \
  /homes/gws/marcs/release/alchemy/src/parser/fol.h \
  /homes/gws/marcs/release/alchemy/src/infer/mrf.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/util/timer.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/infer/groundpredicate.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/lazywalksat.h \
  /homes/gws/marcs/release/alchemy/src/infer/freestoremanager.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwinfo.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/infer/lwutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/intclause.h \
  /homes/gws/marcs/release/alchemy/src/infer/wsutil.h \
  /homes/gws/marcs/release/alchemy/src/infer/maxwalksatparams.h \
  /homes/gws/marcs/release/alchemy/src/infer/convergencetest.h \
  /homes/gws/marcs/release/alchemy/src/util/meanvariance.h \
  /homes/gws/marcs/release/alchemy/src/infer/gelmanconvergencetest.h \
  /homes/gws/marcs/release/alchemy/src/learnstruct/structlearn.h \
  /homes/gws/marcs/release/alchemy/src/logic/clausefactory.h \
  /homes/gws/marcs/release/alchemy/src/logic/mln.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/pseudologlikelihood.h \
  /homes/gws/marcs/release/alchemy/src/util/array.h \
  /homes/gws/marcs/release/alchemy/src/util/random.h \
  /homes/gws/marcs/release/alchemy/src/logic/domain.h \
  /homes/gws/marcs/release/alchemy/src/logic/clause.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/lbfgsb.h \
  /homes/gws/marcs/release/alchemy/src/learnwts/pseudologlikelihood.h

