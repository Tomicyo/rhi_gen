from generator import ProtypeLoader, TemplateGenerator

templ = ProtypeLoader('implement.yml')
templ_gen = TemplateGenerator()
templ_gen.gen(templ, 'build/implement.h')