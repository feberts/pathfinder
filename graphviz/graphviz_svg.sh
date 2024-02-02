#!/bin/bash

dot -Tsvg graphviz_directed.dot -o graphviz_directed.dot.svg
dot -Tsvg graphviz_undirected.dot -o graphviz_undirected.dot.svg
neato -Tsvg graphviz_undirected.dot -o graphviz_undirected.neato.svg
