USER GUIDE:

-You can create any amount of sets

-Sets are automatically loaded and saved upon closing

-If your expression contains the name of a new set, 
a new set will be created with that name.

-Your expression can be any legitimate combination of 
union (|), intersection(&), difference(/), 
assignation(= |= &= /=), brackets and constants({1,2,3}).

-You can only put integer values in constants

EXAMPLES:

Apple = {1, 2, 3, 4, 5}

Bus = Apple \ {2,4}

_et13k = Apple | Bus / Apple&Bus

E = {1,2,3}

E &= Bus

R=(E|Apple)&Bus


Type "help" to see this screen in program
