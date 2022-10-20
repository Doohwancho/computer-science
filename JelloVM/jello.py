#!/usr/bin/env python3

import pprint
import sys
import io

pp = pprint.PrettyPrinter()

CONSTANT_Class              = 7
CONSTANT_Fieldref           = 9
CONSTANT_Methodref          = 10
CONSTANT_InterfaceMethodref = 11
CONSTANT_String             = 8
CONSTANT_Integer            = 3
CONSTANT_Float              = 4
CONSTANT_Long               = 5
CONSTANT_Double             = 6
CONSTANT_NameAndType        = 12
CONSTANT_Utf8               = 1
CONSTANT_MethodHandle       = 15
CONSTANT_MethodType         = 16
CONSTANT_InvokeDynamic      = 18

class_access_flags = [
    ("ACC_PUBLIC"     , 0x0001),
    ("ACC_FINAL"      , 0x0010),
    ("ACC_SUPER"      , 0x0020),
    ("ACC_INTERFACE"  , 0x0200),
    ("ACC_ABSTRACT"   , 0x0400),
    ("ACC_SYNTHETIC"  , 0x1000),
    ("ACC_ANNOTATION" , 0x2000),
    ("ACC_ENUM"       , 0x4000)
]

method_access_flags = [
    ("ACC_PUBLIC", 0x0001),
    ("ACC_PRIVATE", 0x0002),
    ("ACC_PROTECTED", 0x0004),
    ("ACC_STATIC", 0x0008),
    ("ACC_FINAL", 0x0010),
    ("ACC_SYNCHRONIZED", 0x0020),
    ("ACC_BRIDGE", 0x0040),
    ("ACC_VARARGS", 0x0080),
    ("ACC_NATIVE", 0x0100),
    ("ACC_ABSTRACT", 0x0400),
    ("ACC_STRICT", 0x0800),
    ("ACC_SYNTHETIC", 0x1000),
]

def parse_flags(value: int, flags: [(str, int)]) -> [str]:
    return [name for (name, mask) in flags if (value&mask) != 0]

def parse_u1(f): return int.from_bytes(f.read(1), 'big') # parse by 1 byte  
def parse_u2(f): return int.from_bytes(f.read(2), 'big') #'big' as big_endian
def parse_u4(f): return int.from_bytes(f.read(4), 'big')

def parse_attributes(f, count):
    attributes = []
    for j in range(count):
        # attribute_info {
        #     u2 attribute_name_index;
        #     u4 attribute_length;
        #     u1 info[attribute_length];
        # }
        attribute = {}
        attribute['attribute_name_index'] = parse_u2(f)
        attribute_length = parse_u4(f)
        attribute['info'] = f.read(attribute_length)
        attributes.append(attribute)
    return attributes

def parse_class_file(file_path):
    with open(file_path, "rb") as f:
        clazz = {}
        clazz['magic'] = hex(parse_u4(f))
        clazz['minor'] = parse_u2(f)
        clazz['major'] = parse_u2(f)
        constant_pool_count = parse_u2(f)
        constant_pool = []
        for i in range(constant_pool_count-1):
            cp_info = {}
            tag = parse_u1(f)
            if tag == CONSTANT_Methodref:
                cp_info['tag'] = 'CONSTANT_Methodref'
                cp_info['class_index'] = parse_u2(f)
                cp_info['name_and_type_index'] = parse_u2(f)
            elif tag == CONSTANT_Class:
                cp_info['tag'] = 'CONSTANT_Class'
                cp_info['name_index'] = parse_u2(f)
            elif tag == CONSTANT_NameAndType:
                cp_info['tag'] = 'CONSTANT_NameAndType'
                cp_info['name_index'] = parse_u2(f)
                cp_info['descriptor_index'] = parse_u2(f)
            elif tag == CONSTANT_Utf8:
                cp_info['tag'] = 'CONSTANT_Utf8'
                length = parse_u2(f);
                cp_info['bytes'] = f.read(length)
            elif tag == CONSTANT_Fieldref:
                cp_info['tag'] = 'CONSTANT_Fieldref'
                cp_info['class_index'] = parse_u2(f)
                cp_info['name_and_type_index'] = parse_u2(f)
            elif tag == CONSTANT_String:
                cp_info['tag'] = 'CONSTANT_String'
                cp_info['string_index'] = parse_u2(f)
            else:
                raise NotImplementedError(f"Unexpected constant tag {tag} in class file {file_path}")
            constant_pool.append(cp_info)
        clazz['constant_pool'] = constant_pool
        clazz['access_flags'] = parse_flags(parse_u2(f), class_access_flags)
        clazz['this_class'] = parse_u2(f)
        clazz['super_class'] = parse_u2(f)
        interfaces_count = parse_u2(f)
        interfaces = []
        for i in range(interfaces_count):
            raise NotImplementedError("We don't support interfaces")
        clazz['interfaces'] = interfaces
        fields_count = parse_u2(f)
        fields = []
        for i in range(fields_count):
            raise NotImplementedError("We don't support fields")
        clazz['fields'] = fields
        methods_count = parse_u2(f)
        methods = []
        for i in range(methods_count):
            # u2             access_flags;
            # u2             name_index;
            # u2             descriptor_index;
            # u2             attributes_count;
            # attribute_info attributes[attributes_count];
            method = {}
            method['access_flags'] = parse_flags(parse_u2(f), method_access_flags)
            method['name_index'] = parse_u2(f)
            method['descriptor_index'] = parse_u2(f)
            attributes_count = parse_u2(f)
            method['attributes'] = parse_attributes(f, attributes_count)
            methods.append(method)
        clazz['methods'] = methods
        attributes_count = parse_u2(f)
        clazz['attributes'] = parse_attributes(f, attributes_count)
        pp.pprint(clazz)
        return clazz

def find_methods_by_name(clazz, name: bytes):
    return [method
            for method in clazz['methods']
            if clazz['constant_pool'][method['name_index'] - 1]['bytes'] == name]

def find_attributes_by_name(clazz, attributes, name: bytes):
    return [attr
            for attr in attributes
            if clazz['constant_pool'][attr['attribute_name_index'] - 1]['bytes'] == name]

def parse_code_info(info: bytes): # parse java virtual machine instruction 
    code = {}
    with io.BytesIO(info) as f: #io.Bytes.IO opens files as bytes 
        # Code_attribute {
        #     u2 attribute_name_index;
        #     u4 attribute_length;
        #     u2 max_stack;
        #     u2 max_locals;
        #     u4 code_length;
        #     u1 code[code_length];
        #     u2 exception_table_length;
        #     {   u2 start_pc;
        #         u2 end_pc;
        #         u2 handler_pc;
        #         u2 catch_type;
        #     } exception_table[exception_table_length];
        #     u2 attributes_count;
        #     attribute_info attributes[attributes_count];
        # }
        code['max_stack'] = parse_u2(f) # 여기에서 stack이 많이 쌓여서 임계점 넘어가면 stack overflow가 걸리는건가 
        code['max_locals'] = parse_u2(f)
        code_length = parse_u4(f)
        code['code'] = f.read(code_length) # 여기에 유저가 적은 코드가 bytecode로 들어가는 듯
        exception_table_length = parse_u2(f) # throw new Exception은 여기서 처리되나보네 
        # NOTE: parsing the code attribute is not finished
        return code

getstatic_opcode = 0xB2
ldc_opcode = 0x12 # ldc stands for LoaD Constant. it loads item from the constant_pool onto the stack. ex. int, float, java.lang.String, java.lang.Class, etc.  System.out.println("Hello World")에서 "Hello World"부분을 가르킴
invokevirtual_opcode = 0xB6 # stack에 올린걸 실행하는 코드
return_opcode = 0xB1
bipush_opcode = 0x10

def get_name_of_class(clazz, class_index: int) -> str:
    return clazz['constant_pool'][clazz['constant_pool'][class_index - 1]['name_index'] - 1]['bytes'].decode('utf-8')

def get_name_of_member(clazz, name_and_type_index: int) -> str:
    return clazz['constant_pool'][clazz['constant_pool'][name_and_type_index - 1]['name_index'] - 1]['bytes'].decode('utf-8')

def execute_code(clazz, code: bytes):
    stack = []
    with io.BytesIO(code) as f:
        while f.tell() < len(code):
            opcode = parse_u1(f)
            if opcode == getstatic_opcode: # System.out은 static 함수. 이걸 stack에 올림. 
                index = parse_u2(f)
                fieldref = clazz['constant_pool'][index - 1]
                name_of_class = get_name_of_class(clazz, fieldref['class_index'])
                name_of_member = get_name_of_member(clazz, fieldref['name_and_type_index'])
                if name_of_class == 'java/lang/System' and name_of_member == 'out':
                    stack.append({'type': 'FakePrintStream'}) # cross-reference validation위해 stack에 type을 더함. 실제 코드를 더하는게 아니라. 여기에서 타입 잘못쓰면 compile 에러 뜨는거군. 
                else:
                    raise NotImplementedError(f"Unsupported member {name_of_class}/{name_of_member} in getstatic instruction")
            elif opcode == ldc_opcode: #여기서 "Hello World"의 bytecode를 스택에 올림. 
                index = parse_u1(f)
                stack.append({'type': 'Constant', 'const': clazz['constant_pool'][index - 1]})
            elif opcode == invokevirtual_opcode: #여기서 stack에 더한 코드 실행하는군 
                index = parse_u2(f)
                methodref = clazz['constant_pool'][index - 1]
                name_of_class = get_name_of_class(clazz, methodref['class_index'])
                name_of_member = get_name_of_member(clazz, methodref['name_and_type_index']);
                if name_of_class == 'java/io/PrintStream' and name_of_member == 'println': # System.out.println() 하드 코딩했네 ㅋㅋ. print(1+1); 같은거 해도 NotImplementedError남 ㅋㅋ  
                    n = len(stack)
                    if len(stack) < 2:
                        raise RuntimeError('{name_of_class}/{name_of_member} expectes 2 arguments, but provided {n}')
                    obj = stack[len(stack) - 2]
                    if obj['type'] != 'FakePrintStream': #여기서 type check해서 타입 다르면 compile 에러 나는 것
                        raise NotImplementedError(f"Unsupported stream type {obj['type']}")
                    arg = stack[len(stack) - 1]
                    if arg['type'] == 'Constant':
                        if arg['const']['tag'] == 'CONSTANT_String':
                            print(clazz['constant_pool'][arg['const']['string_index'] - 1]['bytes'].decode('utf-8')) # 여기서 print하네 
                        else:
                            raise NotImplementedError(f"println for {arg['const']['tag']} is not implemented")
                    elif arg['type'] == 'Integer':
                        print(arg['value']) # integer이면 여기서 프린트하고 
                    else:
                        raise NotImplementedError(f"Support for {arg['type']} is not implemented")
                else:
                    raise NotImplementedError(f"Unknown method {name_of_class}/{name_of_member} in invokevirtual instruction")
            elif opcode == return_opcode:
                #print(stack) #[{'type': 'FakePrintStream'}, {'type': 'Constant', 'const': {'tag': 'CONSTANT_String', 'string_index': 18}}] 끝나도 stack에 담겨있잖아;;
                # 근데 return 하기 전에 stack에 올린 애들 pop해줘야 하는거 아닌가? 
                return
            elif opcode == bipush_opcode:
                byte = parse_u1(f)
                stack.append({'type': 'Integer', 'value': byte}) #print(1);은 되는데, print(1+1);는 아직 안되네 ㅋㅋ 
            else:
                raise NotImplementedError(f"Unknown opcode {hex(opcode)}")

if __name__ == '__main__':
    program, *args = sys.argv
    if len(args) == 0:
        print(f"Usage: {program} <path/to/Main.class>")
        print(f"ERROR: no path to Main.class was provided")
        exit(1)
    file_path, *args = args
    clazz = parse_class_file(file_path)
    [main] = find_methods_by_name(clazz, b'main') 
    [code] = find_attributes_by_name(clazz, main['attributes'], b'Code')
    code_attrib = parse_code_info(code['info']) #System.out.println("Hello World"); -> b'\x00\x02\x00\x01\x00\x00\x00\t\xb2\x00\x02\x12\x03\xb6\x00\x04\xb1\x00\x00\x00\x01\x00\n\x00\x00\x00\n\x00\x02\x00\x00\x00\x03\x00\x08\x00\x04'
    execute_code(clazz, code_attrib['code']) #clazz에는 parse된 기본 정보들이랑, code_attrib['code']에서 개발자가 적은 코드의 byte를 넘겨줌 
