// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "game.h"
#include "pieces.h"
#include "TextMaker.hpp"
#include "const.hpp"


#include <memory> // For using smart pointers

std::vector<SingleText> outText = {
        {1, {"Pawn", "", "", ""}, 0, 0},
        {1, {"Knight", "", "", ""}, 0, 0},
        {1, {"Bishop", "", "", ""}, 0, 0},
        {1, {"Rook", "", "", ""}, 0, 0},
        {1, {"Queen", "", "", ""}, 0, 0},
        {1, {"King", "", "", ""}, 0, 0},
        {1, {"Demo Mode Chess", "", "", ""}, 0, 0}
        
};

// The uniform buffer objects data structures
// Remember to use the correct alignas(...) value
//        float : alignas(4)
//        vec2  : alignas(8)
//        vec3  : alignas(16)
//        vec4  : alignas(16)
//        mat3  : alignas(16)
//        mat4  : alignas(16)
// Example:
struct UniformBlock {
	alignas(16) glm::mat4 mvpMat;
};

struct UniformColor {
    alignas(4) float trigger;
	alignas(8) glm::vec2 uvColor;
};

struct GlobalUniformBufferObject {
    struct {
        alignas(16) glm::vec3 v;
    } lightDir[5];
    struct {
        alignas(16) glm::vec3 v;
    } lightPos[5];
    alignas(16) glm::vec4 lightColor;
    alignas(4) float cosIn;
    alignas(4) float cosOut;
    alignas(16) glm::vec3 eyePos;
    alignas(16) glm::vec4 eyeDir;
};

// The vertices data structures
// Example
struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
    glm::vec3 normal;
};


// MAIN ! 
class MeshLoader : public BaseProject {
	protected:

	// Current aspect ratio (used by the callback that resized the window
	float Ar;

    GlobalUniformBufferObject gubo;

	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSL, DSL_GLOBAL;

	// Vertex formats
	VertexDescriptor VD;

	// Pipelines [Shader couples]
	Pipeline P;

	// Models, textures and Descriptors (values assigned to the uniforms)
	// Please note that Model objects depends on the corresponding vertex structure
	// Models
	Model<Vertex> MCB, MG, M1, MC[8][8], MP[2][16], MT[36];
	// Descriptor sets
	DescriptorSet DSCB, DSG, DS1, DSC[8][8], DSP[2][16], globalDS, DST[36];
	// Textures
	Texture T1, TB, TW, TT;
	
	// C++ storage for uniform variables
	UniformBlock uboCB, uboG, ubo1, uboC[8][8], uboP[2][16], uboT[36];

    UniformColor colorB[8][8];

    TextMaker txt;

	// Other application parameters
    int piece=-1;
    int currText = 6;
    int currScene = 0;
    //glm::vec3 CamPos = glm::vec3(4.0, 11.5, 19.0);
    //float CamAlpha = 0.0f;
    glm::vec3 CamPos = glm::vec3(4.0, 11.5, -11.0);
    float CamAlpha = glm::radians(-180.0f);
    float CamBeta = glm::radians(-35.0f);

    bool isCameraFixed = false;
    glm::vec3 fixedCamPos = glm::vec3(0.5f, 1.0f, 6.5f);
    glm::vec3 fixedCamDir = glm::vec3(0.0f, 0.0f, -1.0f);


    Game game;
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Project-Chess";
    	windowResizable = GLFW_TRUE;
		//initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};
        initialBackgroundColor = {0.1f, 0.9f, 1.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 170;
		texturesInPool = 170;
		setsInPool = 170;
		
		Ar = (float)windowWidth / (float)windowHeight;
	}
	
	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		Ar = (float)w / (float)h;
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Create a game
		game.init();

        gubo.lightPos[0].v = glm::vec3(4.0f, 5.0f, 4.0f);      // Posizione
        gubo.lightPos[1].v = glm::vec3(0.0f, 5.0f, 0.0f);
        gubo.lightPos[2].v = glm::vec3(0.0f, 5.0f, 8.0f);
        gubo.lightPos[3].v = glm::vec3(8.0f, 5.0f, 8.0f);
        gubo.lightPos[4].v = glm::vec3(8.0f, 5.0f, 0.0f);
        gubo.lightDir[0].v = glm::vec3(0.0f, -1.0f, 0.0f);      // Direzione
        gubo.lightDir[1].v = glm::vec3(0.0f, -1.0f, 0.0f);
        gubo.lightDir[2].v = glm::vec3(0.0f, -1.0f, 0.0f);
        gubo.lightDir[3].v = glm::vec3(0.0f, -1.0f, 0.0f);
        gubo.lightDir[4].v = glm::vec3(0.0f, -1.0f, 0.0f);
        gubo.cosIn = glm::cos(glm::radians(60.0f));        // Angolo interno di cutoff della spotlight
        gubo.cosOut = glm::cos(glm::radians(75.0f));       // Angolo esterno di cutoff della spotlight
        gubo.lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // Colore della spotlight
        gubo.eyePos = CamPos;                             // Posizione della camera
        gubo.eyeDir = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f); // Direzione della camera (modificabile a seconda delle necessità)



        // Descriptor Layouts [what will be passed to the shaders]
		DSL.init(this, {
					// this array contains the bindings:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					//                  using the corresponding Vulkan constant
					// third  element : the pipeline stage where it will be used
					//                  using the corresponding Vulkan constant
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT},
                    {2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS}
				});

        // Global descriptor layout
        DSL_GLOBAL.init(this, {
                {0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });

		// Vertex descriptors
		VD.init(this, {
				  // this array contains the bindings
				  // first  element : the binding number
				  // second element : the stride of this binging
				  // third  element : whether this parameter change per vertex or per instance
				  //                  using the corresponding Vulkan constant
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  // this array contains the location
				  // first  element : the binding number
				  // second element : the location number
				  // third  element : the offset of this element in the memory record
				  // fourth element : the data type of the element
				  //                  using the corresponding Vulkan constant
				  // fifth  elmenet : the size in byte of the element
				  // sixth  element : a constant defining the element usage
				  //                   POSITION - a vec3 with the position
				  //                   NORMAL   - a vec3 with the normal vector
				  //                   UV       - a vec2 with a UV coordinate
				  //                   COLOR    - a vec4 with a RGBA color
				  //                   TANGENT  - a vec4 with the tangent vector
				  //                   OTHER    - anything else
				  //
				  // ***************** DOUBLE CHECK ********************
				  //    That the Vertex data structure you use in the "offsetoff" and
				  //	in the "sizeof" in the previous array, refers to the correct one,
				  //	if you have more than one vertex format!
				  // ***************************************************
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
				         sizeof(glm::vec2), UV},
                  {0, 2, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal),
                   sizeof(glm::vec3), NORMAL}
				});

		// Pipelines [Shader couples]
		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P.init(this, &VD, "shaders/ShaderVert.spv", "shaders/ShaderFrag.spv", {&DSL, &DSL_GLOBAL});
        P.setAdvancedFeatures(VK_COMPARE_OP_LESS_OR_EQUAL, VK_POLYGON_MODE_FILL,
                              VK_CULL_MODE_BACK_BIT, false);

		// Models, textures and Descriptors (values assigned to the uniforms)

		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ or GLTF

        // Creates a mesh with direct enumeration of vertices and indices
        MCB.vertices = {
                //Top
                {{-0.5f, -0.001f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f, -0.001f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 1.0f, 0.0f}},
                {{8.5f, -0.001f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 1.0f, 0.0f}},
                {{8.5f, -0.001f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 1.0f, 0.0f}},
                // Bottom
                {{-0.5f, -1.0f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, -1.0f, 0.0f}},
                {{-0.5f, -1.0f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, -1.0f, 0.0f}},
                {{8.5f, -1.0f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, -1.0f, 0.0f}},
                {{8.5f, -1.0f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, -1.0f, 0.0f}},
                //Left
                {{-0.5f, -0.001f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, -1.0f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, -1.0f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {-1.0f, 0.0f, 0.0f}},
                {{-0.5f, -0.0001f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {-1.0f, 0.0f, 0.0f}},
                //Right
                {{8.5f, -0.001f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {1.0f, 0.0f, 0.0f}},
                {{8.5f, -1.0f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {1.0f, 0.0f, 0.0f}},
                {{8.5f, -1.0f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {1.0f, 0.0f, 0.0f}},
                {{8.5f, -0.001f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {1.0f, 0.0f, 0.0f}},
                //Front
                {{-0.5f, -0.001f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, 1.0f}},
                {{-0.5f, -1.0f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, 1.0f}},
                {{8.5f, -1.0f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, 1.0f}},
                {{8.5f, -0.001f, 8.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, 1.0f}},
                //Back
                {{8.5f, -0.001f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, -1.0f}},
                {{8.5f, -1.0f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, -1.0f}},
                {{-0.5f, -1.0f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, -1.0f}},
                {{-0.5f, -0.001f, -0.5f}, {colorMat[2][0], colorMat[2][1]}, {0.0f, 0.0f, -1.0f}}
        };
        MCB.indices = {
                // Front face
                16,17,18,16,18,19,
                // Back face
                20,21,22,20,22,23,
                // Top face
                0,1,2,0,2,3,
                // Bottom face
                4,6,5,4,7,6,
                // Left face
                8,9,10,8,10,11,
                // Right face
                12,13,14,12,14,15
        };
        MCB.initMesh(this, &VD);

        MG.vertices = {
                //Top
                {{-14.5f, -1.0001f, -14.5f}, {colorMat[3][0], colorMat[3][1]}, {0.0f, 1.0f, 0.0f}},
                {{-14.5f, -1.0001f, 22.5f}, {colorMat[3][0], colorMat[3][1]}, {0.0f, 1.0f, 0.0f}},
                {{22.5f, -1.0001f, 22.5f}, {colorMat[3][0], colorMat[3][1]}, {0.0f, 1.0f, 0.0f}},
                {{22.5f, -1.0001f, -14.5f}, {colorMat[3][0], colorMat[3][1]}, {0.0f, 1.0f, 0.0f}}
        };
        MG.indices = {
                // Top face
                0,1,2,0,2,3
        };
        MG.initMesh(this, &VD);

        int c=1;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){

                MC[i][j].vertices = {
                        {{0.0f+(float)j, 0.0f, 0.0f+(float)i}, {colorMat[c][0], colorMat[c][1]}, {0.0f, 1.0f, 0.0f}},
                        {{0.0f+(float)j, 0.0f, 1.0f+(float)i}, {colorMat[c][0], colorMat[c][1]}, {0.0f, 1.0f, 0.0f}},
                        {{1.0f+(float)j, 0.0f, 1.0f+(float)i}, {colorMat[c][0], colorMat[c][1]}, {0.0f, 1.0f, 0.0f}},
                        {{1.0f+(float)j, 0.0f, 0.0f+(float)i}, {colorMat[c][0], colorMat[c][1]}, {0.0f, 1.0f, 0.0f}},
                };
                MC[i][j].indices = { 0,1,2,0,2,3};

                MC[i][j].initMesh(this, &VD);
                if(j!=7) {
                    if (c == 1) {
                        c = 0;
                    } else if (c == 0) {
                        c = 1;
                    }
                }
                UniformColor color{};
                color.uvColor = {colorMat[4][0], colorMat[4][1]};
                color.trigger = 0.0f;
                colorB[i][j] = color;
            }
        }


        for(int i=0; i<2; i++){
            for(int j=0; j<16; j++){
                if(j==0){
                    MP[i][j].init(this,   &VD, "models/King.obj", OBJ);
                }else if(j==1){
                    MP[i][j].init(this,   &VD, "models/Queen.obj", OBJ);
                }else if(j==2 || j==3){
                    MP[i][j].init(this,   &VD, "models/Bishop.obj", OBJ);
                }else if(j==4 || j==5){
                    MP[i][j].init(this,   &VD, "models/Knight.obj", OBJ);
                }else if(j==6 || j==7){
                    MP[i][j].init(this,   &VD, "models/Rook.obj", OBJ);
                }else{
                    MP[i][j].init(this,   &VD, "models/Pawn.obj", OBJ);
                }
            }
        }

        for(int i=0; i<36; i++){
            MT[i].init(this,   &VD, "models/Pino.obj", OBJ);
        }

		// Create the textures
		// The second parameter is the file name
		T1.init(this, "textures/Textures.png");
        TB.init(this, "textures/ChessPiecesBlack.png");
        TW.init(this, "textures/ChessPiecesWhite.png");
        TT.init(this, "textures/Trees.png");

        txt.init(this, &outText);

		
		// Init local variables
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P.create();

        globalDS.init(this, &DSL_GLOBAL, {
                {0, UNIFORM, sizeof(GlobalUniformBufferObject), nullptr}
        });

		// Here you define the data set
        // the second parameter, is a pointer to the Uniform Set Layout of this set
        // the last parameter is an array, with one element per binding of the set.
        // first  elmenet : the binding number
        // second element : UNIFORM or TEXTURE (an enum) depending on the type
        // third  element : only for UNIFORMs, the size of the corresponding C++ object. For texture, just put 0
        // fourth element : only for TEXTUREs, the pointer to the corresponding texture object. For uniforms, use nullptr

        DSCB.init(this, &DSL, {
                {0, UNIFORM, sizeof(UniformBlock), nullptr},
                {1, TEXTURE, 0, &T1},
                {2, UNIFORM, sizeof(UniformColor), nullptr}
        });

        DSG.init(this, &DSL, {
                {0, UNIFORM, sizeof(UniformBlock), nullptr},
                {1, TEXTURE, 0, &T1},
                {2, UNIFORM, sizeof(UniformColor), nullptr}
        });

        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                DSC[i][j].init(this, &DSL, {
                        {0, UNIFORM, sizeof(UniformBlock), nullptr},
                        {1, TEXTURE, 0, &T1},
                        {2, UNIFORM, sizeof(UniformColor), nullptr}
                });
            }
        }


        for(int i=0; i<2; i++){
            for(int j=0; j<16; j++){
                if(i==0){
                    DSP[i][j].init(this, &DSL, {
                            {0, UNIFORM, sizeof(UniformBlock), nullptr},
                            {1, TEXTURE, 0, &TB},
                            {2, UNIFORM, sizeof(UniformColor), nullptr}
                    });
                }else{
                    DSP[i][j].init(this, &DSL, {
                            {0, UNIFORM, sizeof(UniformBlock), nullptr},
                            {1, TEXTURE, 0, &TW},
                            {2, UNIFORM, sizeof(UniformColor), nullptr}
                    });
                }
            }
        }

        for(int i=0; i<36; i++){
            DST[i].init(this, &DSL, {
                    {0, UNIFORM, sizeof(UniformBlock), nullptr},
                    {1, TEXTURE, 0, &TT},
                    {2, UNIFORM, sizeof(UniformColor), nullptr}
            });
        }

        txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		P.cleanup();

		// Cleanup datasets

        DSCB.cleanup();
        //DS1.cleanup();
        DSG.cleanup();
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                DSC[i][j].cleanup();
            }
        }
        //DS1.cleanup();

        for(int i=0; i<2; i++){
            for(int j=0; j<16; j++){
                DSP[i][j].cleanup();
            }
        }

        for(int i=0; i<36; i++){
            DST[i].cleanup();
        }

        globalDS.cleanup();

        txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {
		// Cleanup textures
		T1.cleanup();
        TB.cleanup();
        TW.cleanup();
        TT.cleanup();
		
		// Cleanup models

        MCB.cleanup();
        //M1.cleanup();
        MG.cleanup();
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                MC[i][j].cleanup();
            }
        }
        //M1.cleanup();
        for(int i=0; i<2; i++){
            for(int j=0; j<16; j++){
                MP[i][j].cleanup();
            }
        }

        for(int i=0; i<36; i++){
            MT[i].cleanup();
        }

		// Cleanup descriptor set layouts
		DSL.cleanup();
        DSL_GLOBAL.cleanup();
		
		// Destroies the pipelines
		P.destroy();
        txt.localCleanup();
    }
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		// binds the pipeline
		P.bind(commandBuffer);
		// For a pipeline object, this command binds the corresponing pipeline to the command buffer passed in its parameter

        globalDS.bind(commandBuffer, P, 1, currentImage);

        // binds the data set
        DSCB.bind(commandBuffer, P, 0, currentImage);
        // For a Dataset object, this command binds the corresponing dataset
        // to the command buffer and pipeline passed in its first and second parameters.
        // The third parameter is the number of the set being bound
        // As described in the Vulkan tutorial, a different dataset is required for each image in the swap chain.
        // This is done automatically in file Starter.hpp, however the command here needs also the index
        // of the current image in the swap chain, passed in its last parameter

        // binds the model
        MCB.bind(commandBuffer);
        // For a Model object, this command binds the corresponing index and vertex buffer
        // to the command buffer passed in its parameter

        // record the drawing command in the command buffer
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MCB.indices.size()), 1, 0, 0, 0);
        // the second parameter is the number of indexes to be drawn. For a Model object,
        // this can be retrieved with the .indices.size() method.

        /*DS1.bind(commandBuffer, P, 0, currentImage);
        M1.bind(commandBuffer);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);*/
        DSG.bind(commandBuffer, P, 0, currentImage);
        MG.bind(commandBuffer);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MG.indices.size()), 1, 0, 0, 0);
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                DSC[i][j].bind(commandBuffer, P, 0, currentImage);
                MC[i][j].bind(commandBuffer);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(MC[i][j].indices.size()), 1, 0, 0, 0);
            }
        }


        for(int i=0; i<2; i++){
            for(int j=0; j<16; j++){
                DSP[i][j].bind(commandBuffer, P, 0, currentImage);
                MP[i][j].bind(commandBuffer);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(MP[i][j].indices.size()), 1, 0, 0, 0);
            }
        }

        for(int i=0; i<36; i++){
                DST[i].bind(commandBuffer, P, 0, currentImage);
                MT[i].bind(commandBuffer);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(MT[i].indices.size()), 1, 0, 0, 0);
        }

        txt.populateCommandBuffer(commandBuffer, currentImage, currText);

	}

    glm::vec3 calculateCenter(const Model<Vertex>& model) {
        glm::vec3 center(0.0f);

        // Somma le posizioni dei 4 vertici del quadrato
        for (int i = 0; i < 4; ++i) {
            center += model.vertices[i].pos;
        }

        // Calcola la media delle posizioni
        center /= 4.0f;

        return center;
    }

    // Get camera pos given a piece
    glm::vec3 getCameraPosition(Piece* piece) {
        float y = (piece->getType() != PieceType::PAWN) ? 0.8f : 1.0f;
        return glm::vec3(static_cast< float >(piece->getPosition().second) +0.5f, y, static_cast< float >(piece->getPosition().first) +0.5f);
    }

    void resetColor(){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                colorB[i][j].trigger = 0.0f;
            }
        }
    }

    void nextPiece(Board &board){
        game.setCurrPiece(board.getNextPiece(game.isPlaying()));
        game.getCurrPiece()->print();
        game.setNextMoove(game.getCurrPiece()->getPosition());
        fixedCamPos = getCameraPosition(game.getCurrPiece());
        if(game.isPlaying()==Color::BLACK) {
            CamAlpha = 0.0f;
        }else{
            CamAlpha = glm::radians(-180.0f);
        }
        CamBeta = 0.0f;
    }

    void previousPiece(Board & board){
        game.setCurrPiece(board.getPrevPiece(game.isPlaying()));
            game.getCurrPiece()->print();
            game.setNextMoove(game.getCurrPiece()->getPosition());
            fixedCamPos = getCameraPosition(game.getCurrPiece());
            if(game.isPlaying()==Color::BLACK) {
                CamAlpha = 0.0f;
            }else{
                CamAlpha = glm::radians(-180.0f);
            }
            CamBeta = 0.0f;
    }

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {

        Board &board = game.getBoard();
		// Standard procedure to quit when the ESC key is pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		// Standard procedure to quit when the SPACE key is pressed
		// Handling of SPACE key for a single press event
		static bool spaceKeyWasPressed = false;
		bool isSpaceKeyPressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

		if (isSpaceKeyPressed && !spaceKeyWasPressed) {
			game.getBoard().displayBoard();
		}
		spaceKeyWasPressed = isSpaceKeyPressed;

        static bool PKeyWasPressed = false;
		bool isPKeyPressed = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);

		if (isPKeyPressed && !PKeyWasPressed) {
            board.printAllPieces(Color::BLACK);
            board.printAllPieces(Color::WHITE);
		}
		PKeyWasPressed = isPKeyPressed;


        static bool MKeyWasPressed = false;
        bool isMKeyPressed = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
        if (isMKeyPressed && !MKeyWasPressed) {
            isCameraFixed = true;
            nextPiece(board);
            currText = (int) game.getCurrPiece()->getType();
            RebuildPipeline();
        }
        MKeyWasPressed = isMKeyPressed;

        static bool NKeyWasPressed = false;
        bool isNKeyPressed = (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS);
        if (isNKeyPressed && !NKeyWasPressed) {
            isCameraFixed = true;
            previousPiece(board);
            currText = (int) game.getCurrPiece()->getType();
            RebuildPipeline();
        }
        NKeyWasPressed = isNKeyPressed;

        static bool WKeyWasPressed = false;
        bool isWKeyPressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
        if (isWKeyPressed && !WKeyWasPressed) {
            std::cout<<"Z"<<std::endl;
            int x = game.getNextMoove().first;
            int y = game.getNextMoove().second;
            int increment = (game.isPlaying() == Color::WHITE) ? 1 : -1;
            game.setNextMoove({x + increment, y});
            resetColor();
            colorB[x + increment][y].trigger = 1.0f;
        }
        WKeyWasPressed = isWKeyPressed;

        static bool AKeyWasPressed = false;
        bool isAKeyPressed = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
        if (isAKeyPressed && !AKeyWasPressed) {
            std::cout<<"Q"<<std::endl;
            int x = game.getNextMoove().first;
            int y = game.getNextMoove().second;
            int increment = (game.isPlaying() == Color::WHITE) ? 1 : -1;
            game.setNextMoove({x, y + increment});
            resetColor();
            colorB[x ][y + increment].trigger = 1.0f;
        }
        AKeyWasPressed = isAKeyPressed;

        static bool SKeyWasPressed = false;
        bool isSKeyPressed = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
        if (isSKeyPressed && !SKeyWasPressed) {
            std::cout<<"S"<<std::endl;
            int x = game.getNextMoove().first;
            int y = game.getNextMoove().second;
            int increment = (game.isPlaying() == Color::WHITE) ? 1 : -1;
            game.setNextMoove({x - increment, y});
            resetColor();
            colorB[x - increment][y].trigger = 1.0f;
        }
        SKeyWasPressed = isSKeyPressed;

        static bool DKeyWasPressed = false;
        bool isDKeyPressed = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
        if (isDKeyPressed && !DKeyWasPressed) {
            std::cout<<"D"<<std::endl;
            int x = game.getNextMoove().first;
            int y = game.getNextMoove().second;
            int increment = (game.isPlaying() == Color::WHITE) ? 1 : -1;
            game.setNextMoove({x, y - increment});
            resetColor();
            colorB[x ][y - increment].trigger = 1.0f;
        }
        DKeyWasPressed = isDKeyPressed;

        static bool ENTERKeyWasPressed = false;
        bool isENTERKeyPressed = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS);
        if (isENTERKeyPressed && !ENTERKeyWasPressed) {
            std::cout<<"ENTER"<<std::endl;
            int x = game.getNextMoove().first;
            int y = game.getNextMoove().second;
            if (board.isLegalMove(game.getCurrPiece(), x, y)){
                board.movePiece(game.getCurrPiece(), x, y);
                resetColor();
                game.swapColor();
                if (board.isCheckmate(game.isPlaying())){
                    std::cout<<"Checkmate"<<std::endl;
                }
                nextPiece(board);
            }
            else{
                std::cout<<"Move is illegal"<<std::endl;
            }
        }
        ENTERKeyWasPressed = isENTERKeyPressed;

        //Z key have to be removed, only for showing purpose
        static bool ZKeyWasPressed = false;
        bool isZKeyPressed = (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);
        if (isZKeyPressed && !ZKeyWasPressed) {
            if(game.isPlaying()==Color::BLACK) {
                CamPos = glm::vec3(4.0, 11.5, 19.0);
            }else{
                CamPos = glm::vec3(4.0, 11.5, -11.0);
            }
            isCameraFixed = false;
            if(game.isPlaying()==Color::BLACK) {
                CamAlpha = 0.0f;
            }else{
                CamAlpha = glm::radians(-180.0f);
            }
            CamBeta = glm::radians(-35.0f);
            currText=6;
            RebuildPipeline();
        }
        ZKeyWasPressed = isZKeyPressed;
		
		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);
		// getSixAxis() is defined in Starter.hpp in the base class.
		// It fills the float point variable passed in its first parameter with the time
		// since the last call to the procedure.
		// It fills vec3 in the second parameters, with three values in the -1,1 range corresponding
		// to motion (with left stick of the gamepad, or ASWD + RF keys on the keyboard)
		// It fills vec3 in the third parameters, with three values in the -1,1 range corresponding
		// to motion (with right stick of the gamepad, or Arrow keys + QE keys on the keyboard, or mouse)
		// If fills the last boolean variable with true if fire has been pressed:
		//          SPACE on the keyboard, A or B button on the Gamepad, Right mouse button

		
		// Parameters
		// Camera FOV-y, Near Plane and Far Plane
        /*(this is a static camera)const float FOVy = glm::radians(90.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.0f;*/

        const float ROT_SPEED = glm::radians(120.0f);
        const float MOVE_SPEED = 2.0f;

        if (!isCameraFixed) {
            CamAlpha = CamAlpha - ROT_SPEED * deltaT * r.y;
            CamBeta = CamBeta - ROT_SPEED * deltaT * r.x;
            CamBeta = CamBeta < glm::radians(-90.0f) ? glm::radians(-90.0f) :
                      (CamBeta > glm::radians(90.0f) ? glm::radians(90.0f) : CamBeta);

            glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
            glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
            CamPos = CamPos + MOVE_SPEED * m.x * ux * deltaT;
            CamPos = CamPos + MOVE_SPEED * m.y * glm::vec3(0, 1, 0) * deltaT;
            CamPos = CamPos + MOVE_SPEED * m.z * uz * deltaT;
        }else {
            CamPos = fixedCamPos;
            CamAlpha = CamAlpha - ROT_SPEED * deltaT * r.y;
            CamBeta = CamBeta - ROT_SPEED * deltaT * r.x;
            CamBeta = CamBeta < glm::radians(-90.0f) ? glm::radians(-90.0f) :
                      (CamBeta > glm::radians(90.0f) ? glm::radians(90.0f) : CamBeta);

            glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0, 1, 0)) * glm::vec4(1, 0, 0, 1);
            glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);
        }


		glm::mat4 World;
        glm::mat4 M = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 50.0f);
        M[1][1] *= -1;

        glm::mat4 Mv =  glm::rotate(glm::mat4(1.0), -CamBeta, glm::vec3(1,0,0)) *
                        glm::rotate(glm::mat4(1.0), -CamAlpha, glm::vec3(0,1,0)) *
                        glm::translate(glm::mat4(1.0), -CamPos);

        glm::mat4 ViewPrj =  M * Mv;
        gubo.eyePos = CamPos;
        gubo.eyeDir = glm::vec4(-CamPos, 0.0f);

        globalDS.map(currentImage, &gubo, sizeof(gubo), 0);

        World = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        uboCB.mvpMat = ViewPrj * World;
        DSCB.map(currentImage, &uboCB, sizeof(uboCB), 0);
        /*World = glm::translate(glm::mat4(1), glm::vec3(-2, -1, -2));
        ubo1.mvpMat = ViewPrj * World;
        DS1.map(currentImage, &ubo1, sizeof(ubo1), 0);*/
        // the .map() method of a DataSet object, requires the current image of the swap chain as first parameter
        // the second parameter is the pointer to the C++ data structure to transfer to the GPU
        // the third parameter is its size
        // the fourth parameter is the location inside the descriptor set of this uniform block

        World = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        uboG.mvpMat = ViewPrj * World;
        DSG.map(currentImage, &uboG, sizeof(uboG), 0);
        
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                World = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
                uboC[i][j].mvpMat = ViewPrj * World;
                DSC[i][j].map(currentImage, &uboC[i][j], sizeof(uboC[i][j]), 0);
                DSC[i][j].map(currentImage, &colorB[i][j], sizeof(colorB[i][j]), 2);
            }
        }


        float xt, zt;

        for(int i=0; i<36; i++){
            xt = positions[i][0];
            zt = positions[i][1];
            World = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(xt, -1.0f, zt)),
                               glm::vec3(0.3f, 0.3f, 0.3f));
            uboT[i].mvpMat = ViewPrj * World;
            DST[i].map(currentImage, &uboT[i], sizeof(uboT[i]), 0);
        }


        const std::vector<Piece*>& pieces = board.getAllPieces();
        for (const Piece* piece : pieces)
        {
            if(piece->isAlive()){
                int i = -1, j = -1;
                if(piece->getColor() == Color::WHITE) {
                        World = glm::scale(glm::translate(glm::mat4(1.0f), calculateCenter(MC[piece->getPosition().first][piece->getPosition().second])),
                                        glm::vec3(10.0f, 10.0f, 10.0f));
                        i = 0;
                        j = piece->getID() - 1;
                    }else{
                        World = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), calculateCenter(MC[piece->getPosition().first][piece->getPosition().second])),
                                        glm::vec3(10.0f, 10.0f, 10.0f)),glm::radians(180.0f),glm::vec3(0,1,0));
                        i = 1;
                        j = piece->getID() - 17;
                    }
                    uboP[i][j].mvpMat = ViewPrj * World;
                    DSP[i][j].map(currentImage, &uboP[i][j], sizeof(uboP[i][j]), 0);
            }
            else{
                int i = -1, j = -1;
                if(piece->getColor() == Color::WHITE) {
                        World = glm::scale(glm::translate(glm::mat4(1.0f), calculateCenter(MC[piece->getPosition().first][piece->getPosition().second])),
                                        glm::vec3(0.0f, 0.0f, 0.0f));
                        i = 0;
                        j = piece->getID() - 1;
                    }else{
                        World = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), calculateCenter(MC[piece->getPosition().first][piece->getPosition().second])),
                                        glm::vec3(0.0f, 0.0f, 0.0f)),glm::radians(180.0f),glm::vec3(0,1,0));
                        i = 1;
                        j = piece->getID() - 17;
                    }
                    uboP[i][j].mvpMat = ViewPrj * World;
                    DSP[i][j].map(currentImage, &uboP[i][j], sizeof(uboP[i][j]), 0);
            }
        }
	}	
};


// This is the main: probably you do not need to touch this!
int main() {
    MeshLoader app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}