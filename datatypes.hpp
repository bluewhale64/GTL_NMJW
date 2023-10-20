class Shader {
    public:
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
        GLuint getshader(){
            return program;
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
        GLuint program = glCreateProgram();
        static unsigned int compile(const char* source, unsigned int type, const char* shadername) {
            if(source != nullptr) {
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
            else {
                printf("Shader %s failed to generate.\n", shadername);
                return -1;
            }
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

class Texture {
    public:
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
        GLuint gettexture(){
            return texture;
        }
        void bindtexture(unsigned int slot) {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        void unbindtexture(void) {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    private:
        GLuint texture;
};

class Renderer {
    public:
        SDL_Window* window;
        SDL_GLContext context;
        Renderer(int width, int height, const char* title, int swapinterval, float red, float green, float blue, float alpha) {
            //Initialize the library
            if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                printf("SDL initiation failed\n");
            }
            printf("SDL2 initiated\n");
                
            window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
            if (window == nullptr) {
                printf("Failed to create SDL2 window\n");
                SDL_Quit();
            }

            context = SDL_GL_CreateContext(window);
            SDL_GL_SetSwapInterval(swapinterval);
    
            if (glewInit() != GLEW_OK) {
                printf("Failed to initialise GLEW.\n");
                SDL_Quit();
            }
    
            printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glClearColor(red, green, blue, alpha);
    
            // Enable depth test
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
    
            glFrontFace(GL_CW);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        ~Renderer() {
            SDL_DestroyWindow(window);
            free(window);
            SDL_Quit();
        }
};