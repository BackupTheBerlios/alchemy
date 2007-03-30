INLINE_LIMIT=-finline-limit=100000
#INLINE_WARN=-Winline
STATIC=-static

######### Profile
#CLFLAGS=-pg -g $(STATIC) -DNDEBUG $(INLINE_WARN) $(INLINE_LIMIT) 

######### Profile & optimize
#CLFLAGS=-pg -g -O3 $(STATIC) -DNDEBUG $(INLINE_WARN) $(INLINE_LIMIT) 

######## Optimize
#CLFLAGS=-O3 $(STATIC) $(INLINE_WARN) $(INLINE_LIMIT)

######## Optimize and turn off asserts
#CLFLAGS=-O3 -DNDEBUG $(STATIC) $(INLINE_WARN) $(INLINE_LIMIT)

########## Debug
CLFLAGS=-g

BASEDIR=..

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
ONLINE=$(SRCDIR)/online

REPLACEFOLCPP=$(PARSER)/replacefolcpp.pl

LIBCINC= -I. -I$(UTIL) -I$(LOGIC) -I$(PARSER) -I$(LEARNWTS) -I$(LEARNSTRUCT) \
	-I$(INFER) -I$(ONLINE)
LIBLINC= -L. -L$(UTIL) -L$(LOGIC) -L$(PARSER) -L$(LEARNWTS) -L$(LEARNSTRUCT) \
	-L$(INFER) -L$(ONLINE)
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
	$(OBJDIR)/groundclause.o

#add .cpp files (including executables') to CPPFILES.
CPPFILES=$(UTIL)/arguments.cpp $(UTIL)/powerset.cpp \
	$(PARSER)/fol.cpp $(PARSER)/listobj.cpp \
	$(LOGIC)/predicatetemplate.cpp $(LOGIC)/functiontemplate.cpp \
	$(LOGIC)/function.cpp $(LOGIC)/predicate.cpp $(LOGIC)/term.cpp \
	$(LOGIC)/domain.cpp  $(LOGIC)/clause.cpp $(LOGIC)/clausesampler.cpp\
	$(INFER)/groundclause.cpp $(LEARNWTS)/learnwts.cpp \
	$(INFER)/infer.cpp $(LEARNSTRUCT)/learnstruct.cpp \
	$(ONLINE)/onlinetest.cpp


##############################################################################
#EXECUTABLES: add executables rules here

all: learnwts infer learnstruct

learnwts: $(OBJDIR)/learnwts.oo
	cp $(OBJDIR)/learnwts.oo $(BIN)/learnwts

infer: $(OBJDIR)/infer.oo
	cp $(OBJDIR)/infer.oo $(BIN)/infer

learnstruct: $(OBJDIR)/learnstruct.oo
	cp $(OBJDIR)/learnstruct.oo $(BIN)/learnstruct

onlinetest: $(OBJDIR)/onlinetest.oo
	cp $(OBJDIR)/onlinetest.oo $(BIN)/onlinetest

$(OBJDIR)/learnwts.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/learnwts.o -c $(LEARNWTS)/learnwts.cpp

$(OBJDIR)/infer.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/infer.o -c $(INFER)/infer.cpp

$(OBJDIR)/learnstruct.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/learnstruct.o -c $(LEARNSTRUCT)/learnstruct.cpp

$(OBJDIR)/onlinetest.o:
	$(GPP) $(CFLAGS) -o $(OBJDIR)/onlinetest.o -c $(ONLINE)/onlinetest.cpp

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

#
# dependencies generated on:  Fri Mar 30 11:56:16 PDT 2007
#
../bin/obj/arguments.o: ../src/util/arguments.cpp ../src/util/arguments.h

../bin/obj/powerset.o: ../src/util/powerset.cpp ../src/util/powerset.h \
  ../src/util/array.h ../src/util/random.h

../bin/obj/fol.o: ../src/parser/fol.cpp ../src/parser/fol.h ../src/logic/mln.h \
  ../src/logic/clause.h ../src/logic/domain.h ../src/util/equalstr.h \
  ../src/util/dualmap.h ../src/util/array.h ../src/util/random.h \
  ../src/util/equalstr.h ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/logic/functiontemplate.h ../src/logic/predicate.h \
  ../src/logic/term.h ../src/util/hash.h ../src/util/hasharray.h \
  ../src/logic/function.h ../src/util/hashstring.h \
  ../src/util/hasharray.h ../src/util/hashlist.h \
  ../src/util/arraysaccessor.h ../src/util/powerset.h \
  ../src/util/multdarray.h ../src/util/hashint.h ../src/logic/database.h \
  ../src/logic/groundpreds.h ../src/infer/groundpredicate.h \
  ../src/logic/domain.h ../src/logic/predicate.h \
  ../src/infer/groundclause.h ../src/util/hasharray.h ../src/util/array.h \
  ../src/util/hash.h ../src/infer/intclause.h \
  ../src/logic/clausesampler.h ../src/logic/truefalsegroundingsstore.h \
  ../src/util/random.h ../src/logic/clausehelper.h \
  ../src/logic/mlnhelper.h ../src/logic/domain.h ../src/logic/database.h \
  ../src/parser/follex.cpp ../src/parser/folhelper.h \
  ../src/parser/strfifolist.h ../src/logic/predicate.h \
  ../src/logic/function.h ../src/logic/clause.h \
  ../src/logic/clausefactory.h ../src/util/hashstring.h \
  ../src/parser/listobj.h ../src/util/array.h \
  ../src/util/arraysaccessor.h ../src/util/timer.h \
  ../src/parser/internals.h

../bin/obj/listobj.o: ../src/parser/listobj.cpp ../src/parser/listobj.h \
  ../src/util/array.h ../src/util/random.h ../src/util/hashstring.h \
  ../src/util/hasharray.h ../src/logic/domain.h ../src/util/equalstr.h \
  ../src/util/dualmap.h ../src/util/array.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/logic/functiontemplate.h ../src/logic/predicate.h \
  ../src/logic/term.h ../src/util/hash.h ../src/util/hasharray.h \
  ../src/logic/function.h ../src/util/arraysaccessor.h

../bin/obj/predicatetemplate.o: ../src/logic/predicatetemplate.cpp \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/util/random.h ../src/logic/domain.h ../src/util/equalstr.h \
  ../src/util/dualmap.h ../src/util/array.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/functiontemplate.h ../src/logic/predicate.h \
  ../src/logic/term.h ../src/util/hash.h ../src/util/hasharray.h \
  ../src/logic/function.h

../bin/obj/functiontemplate.o: ../src/logic/functiontemplate.cpp \
  ../src/logic/functiontemplate.h ../src/logic/predicatetemplate.h \
  ../src/util/array.h ../src/util/random.h ../src/logic/domain.h \
  ../src/util/equalstr.h ../src/util/dualmap.h ../src/util/array.h \
  ../src/util/equalstr.h ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicate.h ../src/logic/term.h ../src/util/hash.h \
  ../src/util/hasharray.h ../src/logic/function.h

../bin/obj/function.o: ../src/logic/function.cpp ../src/logic/function.h \
  ../src/logic/functiontemplate.h ../src/logic/predicatetemplate.h \
  ../src/util/array.h ../src/util/random.h ../src/util/hash.h \
  ../src/util/array.h ../src/logic/term.h ../src/logic/domain.h \
  ../src/util/equalstr.h ../src/util/dualmap.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicate.h ../src/util/hasharray.h

../bin/obj/predicate.o: ../src/logic/predicate.cpp ../src/logic/predicate.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/util/random.h ../src/logic/term.h ../src/util/hash.h \
  ../src/util/array.h ../src/util/hasharray.h ../src/logic/function.h \
  ../src/logic/functiontemplate.h ../src/logic/clause.h \
  ../src/logic/domain.h ../src/util/equalstr.h ../src/util/dualmap.h \
  ../src/util/equalstr.h ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/util/hashstring.h ../src/util/hasharray.h ../src/util/hashlist.h \
  ../src/util/arraysaccessor.h ../src/util/powerset.h \
  ../src/util/multdarray.h ../src/util/hashint.h ../src/logic/database.h \
  ../src/logic/groundpreds.h ../src/infer/groundpredicate.h \
  ../src/logic/domain.h ../src/logic/predicate.h \
  ../src/infer/groundclause.h ../src/util/hasharray.h ../src/util/array.h \
  ../src/util/hash.h ../src/infer/intclause.h \
  ../src/logic/clausesampler.h ../src/logic/truefalsegroundingsstore.h \
  ../src/util/random.h ../src/logic/clausehelper.h

../bin/obj/term.o: ../src/logic/term.cpp ../src/logic/term.h ../src/util/array.h \
  ../src/util/random.h ../src/logic/function.h \
  ../src/logic/functiontemplate.h ../src/logic/predicatetemplate.h \
  ../src/util/hash.h ../src/util/array.h ../src/logic/domain.h \
  ../src/util/equalstr.h ../src/util/dualmap.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicate.h ../src/util/hasharray.h

../bin/obj/domain.o: ../src/logic/domain.cpp ../src/logic/clause.h \
  ../src/logic/domain.h ../src/util/equalstr.h ../src/util/dualmap.h \
  ../src/util/array.h ../src/util/random.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/logic/functiontemplate.h ../src/logic/predicate.h \
  ../src/logic/term.h ../src/util/hash.h ../src/util/hasharray.h \
  ../src/logic/function.h ../src/util/hashstring.h \
  ../src/util/hasharray.h ../src/util/hashlist.h \
  ../src/util/arraysaccessor.h ../src/util/powerset.h \
  ../src/util/multdarray.h ../src/util/hashint.h ../src/logic/database.h \
  ../src/logic/groundpreds.h ../src/infer/groundpredicate.h \
  ../src/logic/domain.h ../src/logic/predicate.h \
  ../src/infer/groundclause.h ../src/util/hasharray.h ../src/util/array.h \
  ../src/util/hash.h ../src/infer/intclause.h \
  ../src/logic/clausesampler.h ../src/logic/truefalsegroundingsstore.h \
  ../src/util/random.h ../src/logic/clausehelper.h ../src/logic/mln.h \
  ../src/logic/mlnhelper.h

../bin/obj/clause.o: ../src/logic/clause.cpp ../src/logic/clause.h \
  ../src/logic/domain.h ../src/util/equalstr.h ../src/util/dualmap.h \
  ../src/util/array.h ../src/util/random.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/logic/functiontemplate.h ../src/logic/predicate.h \
  ../src/logic/term.h ../src/util/hash.h ../src/util/hasharray.h \
  ../src/logic/function.h ../src/util/hashstring.h \
  ../src/util/hasharray.h ../src/util/hashlist.h \
  ../src/util/arraysaccessor.h ../src/util/powerset.h \
  ../src/util/multdarray.h ../src/util/hashint.h ../src/logic/database.h \
  ../src/logic/groundpreds.h ../src/infer/groundpredicate.h \
  ../src/logic/domain.h ../src/logic/predicate.h \
  ../src/infer/groundclause.h ../src/util/hasharray.h ../src/util/array.h \
  ../src/util/hash.h ../src/infer/intclause.h \
  ../src/logic/clausesampler.h ../src/logic/truefalsegroundingsstore.h \
  ../src/util/random.h ../src/logic/clausehelper.h ../src/infer/mrf.h \
  ../src/util/timer.h ../src/logic/mln.h ../src/logic/mlnhelper.h \
  ../src/infer/groundpredicate.h

../bin/obj/clausesampler.o: ../src/logic/clausesampler.cpp \
  ../src/logic/clausesampler.h ../src/logic/database.h \
  ../src/logic/groundpreds.h ../src/logic/predicate.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/util/random.h ../src/logic/term.h ../src/util/hash.h \
  ../src/util/array.h ../src/util/hasharray.h ../src/logic/function.h \
  ../src/logic/functiontemplate.h ../src/logic/domain.h \
  ../src/util/equalstr.h ../src/util/dualmap.h ../src/util/equalstr.h \
  ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/util/arraysaccessor.h ../src/infer/groundpredicate.h \
  ../src/logic/domain.h ../src/logic/predicate.h \
  ../src/infer/groundclause.h ../src/util/hasharray.h ../src/util/array.h \
  ../src/util/hash.h ../src/logic/truefalsegroundingsstore.h \
  ../src/util/random.h ../src/logic/clause.h ../src/util/hashstring.h \
  ../src/util/hasharray.h ../src/util/hashlist.h ../src/util/powerset.h \
  ../src/util/multdarray.h ../src/util/hashint.h ../src/infer/intclause.h \
  ../src/logic/clausehelper.h

../bin/obj/groundclause.o: ../src/infer/groundclause.cpp ../src/infer/groundclause.h \
  ../src/util/hasharray.h ../src/util/random.h ../src/util/array.h \
  ../src/util/hash.h ../src/util/array.h ../src/infer/groundpredicate.h \
  ../src/logic/domain.h ../src/util/equalstr.h ../src/util/dualmap.h \
  ../src/util/equalstr.h ../src/util/constdualmap.h ../src/util/strint.h \
  ../src/logic/predicatetemplate.h ../src/util/array.h \
  ../src/logic/functiontemplate.h ../src/logic/predicate.h \
  ../src/logic/term.h ../src/util/hash.h ../src/util/hasharray.h \
  ../src/logic/function.h ../src/logic/predicate.h ../src/logic/clause.h \
  ../src/logic/domain.h ../src/util/hashstring.h ../src/util/hasharray.h \
  ../src/util/hashlist.h ../src/util/arraysaccessor.h \
  ../src/util/powerset.h ../src/util/multdarray.h ../src/util/hashint.h \
  ../src/logic/database.h ../src/logic/groundpreds.h \
  ../src/infer/groundpredicate.h ../src/infer/intclause.h \
  ../src/logic/clausesampler.h ../src/logic/truefalsegroundingsstore.h \
  ../src/util/random.h ../src/logic/clausehelper.h

../bin/obj/
../bin/obj/
../bin/obj/
../bin/obj/
