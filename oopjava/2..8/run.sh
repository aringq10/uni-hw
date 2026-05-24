#!/bin/bash
mkdir -p target/classes
find src/main/java -name '*.java' -print0 | xargs -0 javac -d target/classes && java -cp target/classes Game
