# Demonstrates access to KTPyTestClass

import nymph_validation as nv

ptc = nv.KTPyTestClass()

print( 'Ok, KTPyTestClass, say hello . . .' )
print( ptc.SayHello() )
print( 'This should print the default value of KTPyTestClass::fValue, which is 5:', ptc.GetValue() )
