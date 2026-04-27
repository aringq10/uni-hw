#!/bin/bash
mkdir -p target/classes
javac -d target/classes src/main/java/npc/*.java src/main/java/npc/**/*.java src/main/java/Game.java && java -cp target/classes Game
