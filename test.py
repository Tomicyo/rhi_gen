from yaml import load

def delegate(obj_func):
    def del_func(obj, path):
        obj_func(obj, path)
        #print obj.__namespace__, path
    return del_func

class EnumWriter(object):
    'Enumeration writer'
    def __set__(self, obj, val):
        print obj, val

class Template(object):
    'a template'
    @delegate
    def __init__(self, path):
        self.__namespace__      = None
        self.__enumlist__       = []
        self.__interfacelist__  = []
        self.__functionlist__   = []
        with open(path) as yml:
            self.__contents__ = load(yml.read())
            yml.close()
        self.__namespace__ = self.__contents__['namespace']
        for (key, val) in self.__contents__.items():
            if key!='namespace':
                if val['type'] == 'enum':
                    self.__enumlist__.append({
                        'enum': key,
                        'val': val
                    })

    @property
    def enums(self):
        return self.__enumlist__

    @property
    def namespace(self):
        return self.__namespace__

    @namespace.setter
    def namespace(self, val):
        if isinstance(val, str):
            self.__namespace__ = val
        else:
            print "Error"

templ = Template('test.yml')

print templ.namespace
print templ.enums