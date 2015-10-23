JC = javac
FLAGS = -classpath java-getopt-1.0.14.jar\:.

build: clean javaprograms

.SUFFIXES: .java .class
.java.class:
		$(JC) $(FLAGS) $*.java

CLASSES = \
		Booking.java \
		HotelInterface.java \
		HotelClient.java \
		HotelImp.java \
		HotelServer.java \
		HotelGateway.java \

javaprograms: $(CLASSES:.java=.class)

clean:
	$(RM) *.class *.o $(ALL)