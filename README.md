Explode game.

The original version was a Windows 3.11 program, with the "single player" algorithm developed by me in C in 1994 when I was still at school. In 2017 I removed the Windows 3.11 code and replaced it with a CLI in C, and added a Java port also with a CLI.
    
To run the C CLI

    gcc -x c *.C -o explode && ./explode

To run the Java CLI, 

    mvn package && java -jar target/explode-1.0-SNAPSHOT.jar
