# afj-interpreter
An interpreter for AFJ class in SS 2016.


## Limitations 

Max input stream (2GB well, whatever `numeric_limits<int>::max()` says).

Any program that never ends and uses W instruction will never write anything to either file (-o) or print anything (-p) because while loop never ends therefore printing/writing to file function is never called.