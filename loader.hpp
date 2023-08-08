class Loader {
    private:
        const char footprint[13] = "NMJW_MDLFMT_";
        GLfloat* vertexbuffer;
        GLuint* indexbuffer;
        uint32_t MODEL_TYPE;
        uint32_t FILE_SIZE;
        uint32_t NUMBER_OF_FLOATS;
        uint16_t FLOAT_SIZE;
        uint32_t FLOAT_START_OFFSET;
        uint32_t NUMBER_OF_INDICES;
        uint16_t INDEX_SIZE;
        uint32_t INDEX_START_OFFSET;
    public:
        Loader(){
            printf("Loader constructed.\n");
        }
        int loadfail(const char* errormessage, FILE* file){
            fprintf(stderr, errormessage);
            fclose(file);
            free(vertexbuffer);
            free(indexbuffer);
            return -1;
        }
        int load_0000(FILE* file){
            fseek(file, 20, SEEK_SET);
            fread(&NUMBER_OF_FLOATS, 4, 1, file);
            fread(&FLOAT_SIZE, 2, 1, file);
            fread(&FLOAT_START_OFFSET, 4, 1, file);
            fread(&NUMBER_OF_INDICES, 4, 1, file);
            fread(&INDEX_SIZE, 2, 1, file);
            fread(&INDEX_START_OFFSET, 4, 1, file); 
            vertexbuffer = (GLfloat*) malloc(NUMBER_OF_FLOATS  * 4);
            if(vertexbuffer == nullptr){
                return loadfail("ERROR: Vertex reading buffer could not be allocated memory.\n", file);
            }
            fseek(file, FLOAT_START_OFFSET, SEEK_SET);
            if(FLOAT_SIZE == 8){
                double store;
                for(int i = 0; i < NUMBER_OF_FLOATS; i++){
                    fread(&store, FLOAT_SIZE, 1, file);
                    vertexbuffer[i] = (GLfloat)store;
                }
            }
            else if(FLOAT_SIZE == 4){
                fread(vertexbuffer, FLOAT_SIZE, NUMBER_OF_FLOATS, file);
            }
            else{
                return loadfail("ERROR: Invalid float size.\n", file);
            }
            fseek(file, INDEX_START_OFFSET, SEEK_SET);
            if(INDEX_SIZE == 2){
                indexbuffer  = (GLuint*) calloc(NUMBER_OF_INDICES, 4);
                if(indexbuffer == nullptr){
                    return loadfail("ERROR: Index reading buffer could not be allocated memory.\n", file);
                }
                for(int i = 0; i < NUMBER_OF_INDICES; i++){
                    fread(&indexbuffer[i], INDEX_SIZE, 1, file);
                }
            }
            else if(INDEX_SIZE == 4){
                indexbuffer  = (GLuint*) malloc(NUMBER_OF_INDICES * 4);
                if(indexbuffer == nullptr){
                    return loadfail("ERROR: Index reading buffer could not be allocated memory.\n", file);
                }
                fread(indexbuffer, INDEX_SIZE, NUMBER_OF_INDICES, file);
            }
            else{
                return loadfail("ERROR: Invalid index size.\n", file);
            }
            fclose(file);
            return 0;
        }
        int load(const char* filename){
            FILE* f = fopen(filename, "rb");
            if(f == nullptr){
                printf("ERROR: %s file could not be found or opened.\n", filename);
                return -1;
            }
            char test_footprint[12];
            fread(test_footprint, 1, 12, f);
            if(memcmp(footprint, test_footprint, 12) != 0){
                printf("ERROR: %s file format unrecognised.\n", filename);
                fclose(f);
                return -1;
            }

            fread(&MODEL_TYPE, 4, 1, f);
            fread(&FILE_SIZE, 4, 1, f);
            printf("Read contents of file %s (%d bytes)\n", filename, FILE_SIZE);

            if(MODEL_TYPE == 0b0000){
                return load_0000(f);
            }
            else{
                printf("ERROR: %s model format unrecognised.\n", filename);
                fclose(f);
                return -1;
            }
        } 
        uint16_t getfloatsize(){
            return FLOAT_SIZE;
        }
        uint16_t getindexsize(){
            return INDEX_SIZE;
        }
        uint32_t getfloatcount(){
            return NUMBER_OF_FLOATS;
        }
        uint32_t getindexcount(){
            return NUMBER_OF_INDICES;
        }
        GLfloat* getvertexbuffer(){
            return vertexbuffer;
        }
        GLuint* getindexbuffer(){
            return indexbuffer;
        }
        ~Loader(){
            free(vertexbuffer);
            free(indexbuffer);
        }
};