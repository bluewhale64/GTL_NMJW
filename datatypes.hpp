class Shader
{
public:
    GLuint program = glCreateProgram();
    Shader(const char* VertexShader, const char* FragmentShader) {
        unsigned int vertexshader = compile(parse(VertexShader), GL_VERTEX_SHADER, VertexShader);
        unsigned int fragmentshader = compile(parse(FragmentShader), GL_FRAGMENT_SHADER, FragmentShader);
        glAttachShader(program, vertexshader);
        glAttachShader(program, fragmentshader);
        glLinkProgram(program);
        glValidateProgram(program);
        glDeleteShader(vertexshader);
        glDeleteShader(fragmentshader);
    }
    ~Shader() {
        glDeleteProgram(program);
    }
    void bind() {
        glUseProgram(program);
    }
    void unbind() {
        glUseProgram(0);
    }
    GLint getuniformlocation(const char* uniformname) {
        GLint location = glGetUniformLocation(program, uniformname);
        if (location == -1) {
            printf("WARNING: Uniform \"%s\" does not exist!\n", (char*)uniformname);
        }
        return location;
    }
    void setuniform4f(const char* uniformname, float v0, float v1, float v2, float v3) {
        glUniform4f(getuniformlocation(uniformname), v0, v1, v2, v3);
    }

    void setuniform1i(const char* uniformname, int i0) {
        glUniform1i(getuniformlocation(uniformname), i0);
    }

    void setuniformmat4f(const char* uniformname, glm::mat4& matrix) {
        glUniformMatrix4fv(getuniformlocation(uniformname), 1, GL_FALSE, &matrix[0][0]);
    }

private:
    static unsigned int compile(const char* source, unsigned int type, const char* shadername) {
        unsigned int id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int len;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            char* message = (char*)malloc(len * sizeof(char));
            glGetShaderInfoLog(id, len, &len, message);
            printf("%s failed to compile!\n", shadername);
            printf("%s\n", message);
            glDeleteShader(id);
            return 0;
        }
        return id;
    }
    const char* parse(const char* filepath) {
        FILE* fp;
        fp = fopen(filepath, "rb");
        if (fp != nullptr) {
            fseek(fp, 0, SEEK_END);
            int length = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            char* shadercode = (char*)malloc((length + 1) * sizeof(char));
            if (shadercode) {
                shadercode[length] = '\0';
                fread(shadercode, 1, length, fp);
                fclose(fp);
                return(shadercode);
            }
            else {
                printf("const char* array shadercode could not be allocated memory, and was a nullptr pointer.\n");
                return(nullptr);
            }
        }
        else {
            printf("Shader file %s could not be found/opened.", filepath);
            return nullptr;
        }
    }
};

class Texture
{
public:
    GLuint texture;
    Texture(const char* imagefile, GLint texture_mag_filter, GLint texture_min_filter, GLint texture_wrap_s, GLint texture_wrap_t, float* border_colour) {
        int width, height, bpp;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* localbuffer = stbi_load(imagefile, &width, &height, &bpp, 4);
        if (!localbuffer) {
            printf("Image file %s could not be found/opened\n", imagefile);
        }
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap_t);
        if ((texture_wrap_s == GL_CLAMP_TO_EDGE || texture_wrap_t == GL_CLAMP_TO_EDGE) && border_colour != nullptr) {
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour);
        }
        else if (texture_wrap_s == GL_CLAMP_TO_EDGE || texture_wrap_t == GL_CLAMP_TO_EDGE) {
            float border[4] = { 0,0,0,0 };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localbuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (localbuffer) {
            stbi_image_free(localbuffer);
        }
    }
    ~Texture() {
        glDeleteTextures(1, &texture);
    }
    void bindtexture(unsigned int slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    void unbindtexture(void) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

class Model {
protected:
    float* vertices;
    unsigned int* indices;
    unsigned int indexnum;
    GLint matloc = 0;
    GLint arrayloc = 0;
    GLint samplers[8] = {};
    GLuint textures[8] = {};
    GLuint shader;
    unsigned int vertexbuffer;
    unsigned int indexbuffer;
    unsigned int vao;
    unsigned int texnum;
    GLfloat* MVP;
public:
    Model(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp) {
        shader = modelshader->program;
        //glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        matloc = glGetUniformLocation(shader, "MVP");
        arrayloc = glGetUniformLocation(shader, "u_texarray");
        texnum = number_of_textures;
        MVP = (GLfloat*)mvp;
        for (uint8_t i = 0; i < texnum; i++) {
            textures[i] = (*texturearray[i]).texture;
        }
        for (uint8_t i = 0; i < texnum; i++) {
            samplers[i] = i;
        }
        vertices = vertices_in;
        indices = indices_in;
        indexnum = number_of_indices;
        floatbuffer(GL_ARRAY_BUFFER, 1, &vertexbuffer, vertices, number_of_vertices * sizeof(float));
        vertexarray(1, &vao);
        unsignedintbuffer(GL_ELEMENT_ARRAY_BUFFER, 1, &indexbuffer, indices, number_of_indices * sizeof(float));
    }
    virtual void draw() {
        for (uint8_t i = 0; i < texnum; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        glUseProgram(shader);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

        glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
        glUniform1iv(arrayloc, texnum, samplers);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

        glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
    virtual ~Model() {}
};

class DispModel : public Model {
private:
    unsigned int vertnum;
    float scroll_x1;
    float scroll_y1;
    float scroll_x2;
    float scroll_y2;
public:

    DispModel(float* vertices_in, int16_t number_of_vertices, unsigned int* indices_in, uint16_t number_of_indices, Texture** texturearray, uint8_t number_of_textures, Shader* modelshader, glm::mat4* mvp, float x1, float y1, float x2, float y2) : Model(vertices_in, number_of_vertices, indices_in, number_of_indices, texturearray, number_of_textures, modelshader, mvp) {
        scroll_x1 = x1;
        scroll_y1 = y1;
        scroll_x2 = x2;
        scroll_y2 = y2;
        vertnum = number_of_vertices / 9;
    }
    void draw() override {
        for (uint8_t i = 0; i < texnum; i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
        }
        glUseProgram(shader);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);

        glUniformMatrix4fv(matloc, 1, GL_FALSE, MVP);
        glUniform1iv(arrayloc, texnum, samplers);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));

        glDrawElements(GL_TRIANGLES, indexnum, GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        //Scroll the texture coordinates
        for (unsigned int i = 0; i < vertnum; i++) {
            vertices[(9 * i) + 3] += scroll_x1;
            vertices[(9 * i) + 4] += scroll_y1;
            vertices[(9 * i) + 5] += scroll_x2;
            vertices[(9 * i) + 6] += scroll_y2;
            //Possibility of float overflow, but will only happen after a long time and will quickly snap back to normal.
        }
        floatbuffer(GL_ARRAY_BUFFER, 1, &vertexbuffer, vertices, vertnum * sizeof(float) * 9);
    }
    ~DispModel() override {}
};

class Renderer {
public:
    GLFWwindow* window;
    Renderer(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, int swapinterval, float red, float green, float blue, float alpha) {
        //Initialize the library
        if (!glfwInit()) {
            printf("GLFW initiation failed\n");
        }
        printf("GLWF initiated\n");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


        //Create a windowed mode window and its OpenGL context
        window = glfwCreateWindow(width, height, title, monitor, share);
        if (window == nullptr) {
            printf("Failed to create GLFW window\n");
            glfwTerminate();
        }

        //Make the window's context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(swapinterval);

        if (glewInit() != GLEW_OK) {
            printf("Oh dear! It seems that for some reason, GLEW didn't initialise correctly.\n");
            glfwTerminate();
        }

        printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glClearColor(red, green, blue, alpha);
        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        
        /*
        // Hide the mouse and enable unlimited mouvement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // Set the mouse at the center of the screen
        glfwPollEvents();
        glfwSetCursorPos(window, width / 2, height / 2);
        */

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glFrontFace(GL_CW);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    ~Renderer() {
        free(window);
    }
};