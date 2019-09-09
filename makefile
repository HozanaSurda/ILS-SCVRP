#detecta se o sistema eh osx ou ubuntu
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    SYSTEM = x86-64_osx
    BCFLAGS = -std=c++11
    LIBFORMAT = static_pic
    CPLEXDIR      = /Applications/CPLEX_Studio128/cplex
    CONCERTDIR    = /Applications/CPLEX_Studio128/concert
else
    SYSTEM = x86-64_linux
    BCFLAGS = -std=c++11
    LIBFORMAT = static_pic
    CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio128/cplex
    CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio128/concert
endif

####diretorios com as libs do cplex
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

#### define o compilador
CPPC = g++
#############################

#### opcoes de compilacao e includes
CCOPT = $(BITS_OPTION) -O3 -g -fPIC -fexceptions -DNDEBUG -DIL_STD -DRETURN_CLIQUES_ONE_BY_ONE $(BCFLAGS) -Wall
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I./../quickc/
#############################

#### flags do linker
ifeq ($(UNAME),Darwin)
    CCLNFLAGS = -L./../quickc/ -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread
else
    CCLNFLAGS = -L./../quickc/ -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -ldl
endif
#############################

#### diretorios com os source files e com os objs files
SRCDIR = src
OBJDIR = obj
#############################

#### lista de todos os srcs e todos os objs
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
#############################

#### regra principal, gera o executavel
exe: $(OBJS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CPPC) $(BCFLAGS) $(BITS_OPTION) $(OBJS) -o $@ $(CCLNFLAGS)
############################

#inclui os arquivos de dependencias
-include $(OBJS:.o=.d)

#regra para cada arquivo objeto: compila e gera o arquivo de dependencias do arquivo objeto
#cada arquivo objeto depende do .c e dos headers (informacao dos header esta no arquivo de dependencias gerado pelo compiler)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CPPC) $(CCFLAGS) -c $< -o $@
	@echo  "\033[32m \ncreating $< dependency file: \033[0m"
	$(CPPC) $(CCFLAGS) -std=c++0x  -MM $< > $(basename $@).d
	@mv -f $(basename $@).d $(basename $@).d.tmp #proximas tres linhas colocam o diretorio no arquivo de dependencias (g++ nao coloca, surprisingly!)
	@sed -e 's|.*:|$(basename $@).o:|' < $(basename $@).d.tmp > $(basename $@).d
	@rm -f $(basename $@).d.tmp

#delete objetos e arquivos de dependencia
clean:
	@echo "\033[31mcleaning obj and outs directories and claw binary\033[0m"
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d
	@rm -f exe
rebuild: clean exe

