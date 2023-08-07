class Loader {
    private:
        const char footprint[13] = "NMJW_MDLFMT_";
        float* vertexbuffer;
        unsigned int* indexbuffer;
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
        int load_0000(FILE* file){
            fseek(file, 20, SEEK_SET);
            fread(&NUMBER_OF_FLOATS, 4, 1, file);
            fread(&FLOAT_SIZE, 2, 1, file);
            fread(&FLOAT_START_OFFSET, 4, 1, file);
            fread(&NUMBER_OF_INDICES, 4, 1, file);
            fread(&INDEX_SIZE, 2, 1, file);
            fread(&INDEX_START_OFFSET, 4, 1, file); 
            vertexbuffer = (float*)    malloc(NUMBER_OF_FLOATS  * FLOAT_SIZE);
            indexbuffer  = (unsigned int*) malloc(NUMBER_OF_INDICES * INDEX_SIZE);
            if(vertexbuffer == nullptr || indexbuffer == nullptr){
                printf("ERROR: Float or Index reading buffer could not be allocated memory.\n");
                fclose(file);
                free(vertexbuffer);
                free(indexbuffer);
                return -1;
            }
            fseek(file, FLOAT_START_OFFSET, SEEK_SET);
            fread(vertexbuffer, FLOAT_SIZE, NUMBER_OF_FLOATS, file);
            fseek(file, INDEX_START_OFFSET, SEEK_SET);
            fread(indexbuffer, INDEX_SIZE, NUMBER_OF_INDICES, file);
            fclose(file);
            /*
            for(int i = 0; i < 30; i++){
                printf("%d\n", indexbuffer[NUMBER_OF_INDICES-1-i]);
            }
            */
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
        float* getvertexbuffer(){
            return vertexbuffer;
        }
        unsigned int* getindexbuffer(){
            return indexbuffer;
        }
        ~Loader(){
            ;
        }
};