#include "shader.h"

Shader::Shader(const char* vertex_path, const char* fragment_path)
{
	//装入shader代码字符串的两个string
	std::string vertex_code;
	std::string fragment_code;

	//用于读取VS跟FS文件的in file stream
	std::ifstream vertex_shader_file;
	std::ifstream fragment_shader_file;

	//保证ifstream遇到问题可以抛出异常
	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//打开文件
		vertex_shader_file.open(vertex_path);
		fragment_shader_file.open(fragment_path);

		//将文件输入流中的字符串输入到string stream里
		std::stringstream vertex_shader_stream;
		std::stringstream fragment_shader_stream;
		vertex_shader_stream << vertex_shader_file.rdbuf();
		fragment_shader_stream << fragment_shader_file.rdbuf();

		//关闭文件
		vertex_shader_file.close();
		fragment_shader_file.close();

		//将字符串从string stream中读取出来 转化到code string当中
		vertex_code = vertex_shader_stream.str();
		fragment_code = fragment_shader_stream.str();
	}
	catch (std::ifstream::failure& error)
	{
		std::cerr << "ERROR: Shader File Error" << error.what() << std::endl;
	}

    const char* vertex_shader_source = vertex_code.c_str();
    const char* fragment_shader_source = fragment_code.c_str();


   //---------------编译-----------------
   // 创建 Shader 程序（vs、fs）
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    //为 shader 程序输入 shader 代码
    glShaderSource(vertex, 1, &vertex_shader_source, NULL);
    glShaderSource(fragment, 1, &fragment_shader_source, NULL);

    glCompileShader(vertex);//编译shader
    check_shader_errors(vertex, Type::COMPILE_VERTEX);

    glCompileShader(fragment);
    check_shader_errors(fragment, Type::COMPILE_FRAGMENT);

    //---------------链接-----------------
    //创建一个Program壳子
    _program = 0;
    _program = glCreateProgram();

    //6 将vs与fs编译好的结果放到program这个壳子里
    glAttachShader(_program, vertex);
    glAttachShader(_program, fragment);

    //7 执行program的链接操作，形成最终可执行shader程序
    glLinkProgram(_program);

    //检查链接错误
    check_shader_errors(_program, Type::LINK_PROGRAM);

    //shader链接完之后要清理,最终想拿到的是program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{

}

void Shader::begin()
{
	GL_CALL(glUseProgram(_program));
}

void Shader::end()
{
	GL_CALL(glUseProgram(0));
}

void Shader::set_float(const std::string& name, float value)
{
    //1 通过名称拿到Uniform变量的Location
    GLint location = glGetUniformLocation(_program, name.c_str());

    //通过Location更新Uniform变量
    GL_CALL(glUniform1f(location,value));
}

void Shader::set_vector3(const std::string& name, float x, float y, float z)
{
    //1 通过名称拿到Uniform变量的Location
    GLint location = glGetUniformLocation(_program, name.c_str());

    GL_CALL(glUniform3f(location,x,y,z));
}

void Shader::set_vector3(const std::string& name, const float* values_3)
{
    //1 通过名称拿到Uniform变量的Location
    GLint location = glGetUniformLocation(_program, name.c_str());

    //第二个参数：你当前要更新的uniform变量如果是数组，数组里包括多少个向量vec3
    GL_CALL(glUniform3fv(location, 1, values_3));
}

void Shader::set_int(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(_program, name.c_str());

    GL_CALL(glUniform1i(location, value));
}



void Shader::check_shader_errors(GLuint target, Type type)
{
    int success = 0;
    char infoLog[1024];//该字符串用来承接错误与否并输出

    if (type == Type::COMPILE_FRAGMENT)
    {
        glGetShaderiv(target, GL_COMPILE_STATUS, &success);// 检查编译结果，并把结果放在success里面
        if (!success)
        {
            glGetShaderInfoLog(target, 1024, NULL, infoLog);
            std::cerr << "Error: SHADER COMPILE_FRAGMENT ERROR" << "\n" << infoLog << std::endl;
        }
    }
    else if (type == Type::COMPILE_VERTEX)
    {
        glGetShaderiv(target, GL_COMPILE_STATUS, &success);// 检查编译结果，并把结果放在success里面
        if (!success)
        {
            glGetShaderInfoLog(target, 1024, NULL, infoLog);
            std::cerr << "Error: SHADER COMPILECOMPILE_VERTEX ERROR" << "\n" << infoLog << std::endl;
        }

    }
    else if(type == Type::LINK_PROGRAM)
    {
        glGetProgramiv(target, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(target, 1024, NULL, infoLog);
            std::cerr << "Error: SHADER LINK ERROR " << "\n" << infoLog << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: Check Shader Errors Type Is Wrong" << std::endl;
    }

}

