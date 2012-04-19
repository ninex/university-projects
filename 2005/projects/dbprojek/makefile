NAME = DB
JAVA = jikes
FLAGS =
FILE1 = window.java
FILE2 = Queries.java
FILE3 = projek.java
FILE4 = Bespreking.java
FILE5 = SQLconnect.java
FILE6 = Search.java
FILE7 = plaas.java
CLASS1 = window.class
CLASS2 = Queries.class
CLASS3 = projek.class
CLASS4 = Bespreking.class
CLASS5 = SQLconnect.class
CLASS6 = Search.class
CLASS7 = plaas.class
RM = rm
CLASSES = $(CLASS1) $(CLASS2) $(CLASS3) $(CLASS4) $(CLASS5) $(CLASS6) $(CLASS7)
FILES = $(FILE1) $(FILE2) $(FILE3) $(FILE4) $(FILE5) $(FILE6) $(FILE7)

all:$(NAME)

$(NAME): $(CLASSES)


$(CLASS1) : $(FILE1)
	$(JAVA) $(FLAGS) $(FILE1)

$(CLASS2) : $(FILE2)
	$(JAVA) $(FLAGS) $(FILE2)

$(CLASS3) : $(FILE3)
	$(JAVA) $(FLAGS) $(FILE3)

$(CLASS4) : $(FILE4)
	$(JAVA) $(FLAGS) $(FILE4)

$(CLASS5) : $(FILE5)
	$(JAVA) $(FLAGS) $(FILE5)

$(CLASS6) : $(FILE6)
	$(JAVA) $(FLAGS) $(FILE6)

$(CLASS7) : $(FILE7)
	$(JAVA) $(FLAGS) $(FILE7)


clean:
	$(RM) *.class
