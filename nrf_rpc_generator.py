'''

Function:

f() --------------------|-----> f_rpc_handler()
f_rpc_rsp(f_rpc_res*) <-|------

'''


class Parser:

    def __init__(self, conf):
        pass

    def parse(self, file):
        pass

    def get_functions(self, input_file_only=False, defined_only=False, annotated_only=False):
        pass

    def get_structures(self, input_file_only=False):
        pass

    def get_variables(self, input_file_only=False):
        pass

    def get_annotations(self):
        pass

    def get_typedefs(self):
        pass


class UnitGenerator:
    
    def generate(self, code_blocks): # code_blocks also contains information if unit has header/footer
        pass


class Function(UnitGenerator):

    def __init__(self):
        pass

    def get_annotations(self):
        pass


class Structure(UnitGenerator):
    pass


class Placeholder(UnitGenerator):
    pass


class Annotation(UnitGenerator):
    pass

