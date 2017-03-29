# Javascript Binding Generator
from loader import ProtypeLoader
from name import NameUtil, make_enum_name, make_enum_value, make_struct_name, make_name_XXX_XXX

class JSGen(object):

    def __init__(self):
        print('')
        self.ns = ''
        self.type_tree = {}

    def gen(self, templ, outpath = ''):
        self.type_tree = templ.type_tree
        self.header = open('test.h', 'w')
        self.source = open('test.cpp', 'w')
        self.header.write('#ifndef __TEST__\n')
        self.header.write('#define __TEST__\n')
        self.header.write('#include <v8.h>\n')
        self.source.write('#include \"test.h\"\n')
        global_templ_handles = []
        header_function_entries = [] 
        for (ns, type_) in self.type_tree.items():
            for (t_name, t_node) in type_['interface'].items():
                handle_name = NameUtil['struct'](ns, t_name)
                global_templ_handles.append('\nv8::Global<v8::ObjectTemplate> g{0}Templ;'.format(handle_name))
                if 'functions' in t_node:
                    fns = t_node['functions']
                    for fn in fns:
                        if isinstance(fn, dict):
                            for (fn_name, fn_val) in fn.items():
                                v8_function_name = self.make_v8_method_name(ns, t_name, fn_name)
                                header_function_entries.append('\nvoid {0}(const v8::FunctionCallbackInfo<v8::Value>& args);'.format(v8_function_name))
                        else:
                            v8_function_name = self.make_v8_method_name(ns, t_name, fn)
                            header_function_entries.append('\nvoid {0}(const v8::FunctionCallbackInfo<v8::Value>& args);'.format(v8_function_name))
                            

        # write the header
        for header_func in header_function_entries:
            self.header.write(header_func)
        self.header.write('\n#endif')

        for handle in global_templ_handles:
            self.source.write(handle)

    def make_v8_method_name(self, ns, inf, fn):
        return '_'.join(' '.join([inf, fn]).split(' '))

    def travel_interface_by_name(self, name):
        for (ns, type_) in self.type_tree.items():
            if t_name in type_['interface']:
                return type_['interface'][t_name]
        return None
    
    def make_js_method_name(self, fn_name):
        return make_name_axxBxx(fn_name)

    def make_function_impl(self, ns, inf, function, ret, params):
        'void {0}(const v8::FunctionCallbackInfo<v8::Value>& args)'.format(make_v8_method_name(ns, inf, function))
        'Local<External> holder = Local<External>::Cast(args.Holder()->GetInternalField(0));\n'
        '{0}* ptr = ({0}*)holder->Value();\n'.format(NameUtil['interface'](ns, inf))
        # expand args
        for i in range(0, len(params)):
            'auto a1 = args[{0}]'.format(i)
            struct_members = [] # if is struct, expand it
            for m in struct_members:
                'a1->Get(\'{0}\')'.format(m)
            # if is pointer/External, get internal ptr

        'ptr->{0}()' # call function with args
        '' # make template
        '' # new instance from template
        'args.GetReturnValue().Set(ret)' # return result


protype = ProtypeLoader('../sappheiros.yml')
jsGen = JSGen()
jsGen.gen(protype)




