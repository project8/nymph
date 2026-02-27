

#from nymph import registrars
from nymph_bindings import processor as nb_proc

registrars = {}

def register(module_name: str, type: str, type_name: str):
    print(f"Registring {module_name}.{type} under {type_name}")
    registrars[type_name] = nb_proc._register(module_name, type, type_name)


def create(type_name: str, name: str):
    print(f"Creating processor of type {type_name} called {name}")
    return nb_proc._create(type_name, name);
