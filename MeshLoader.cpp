// This has been adapted from the Vulkan tutorial

#include "Starter.hpp"
#include "headers/game.h"

#include <memory> // For using smart pointers

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

// The vertices data structures
// Example
struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
};





// MAIN ! 
class MeshLoader : public BaseProject {
	protected:

	// Current aspect ratio (used by the callback that resized the window
	float Ar;

	// Descriptor Layouts ["classes" of what will be passed to the shaders]
	DescriptorSetLayout DSL;

	// Vertex formats
	VertexDescriptor VD;

	// Pipelines [Shader couples]
	Pipeline P;

	// Models, textures and Descriptors (values assigned to the uniforms)
	// Please note that Model objects depends on the corresponding vertex structure
	// Models
	Model<Vertex> MCB, M1, MC[8][8];
	// Descriptor sets
	DescriptorSet DSCB, DS1, DSC[8][8];
	// Textures
	Texture T1, T2;
	
	// C++ storage for uniform variables
	UniformBlock uboCB, ubo1, uboC[8][8];

	// Other application parameters
    int currScene = 0;
    glm::vec3 CamPos = glm::vec3(2.0, 2.0, 15.0);
    float CamAlpha = 0.0f;
    float CamBeta = 0.0f;

	Game game;
	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 800;
		windowHeight = 600;
		windowTitle = "Project-Chess";
    	windowResizable = GLFW_TRUE;
		initialBackgroundColor = {0.0f, 1.0f, 1.0f, 1.0f};
		
		// Descriptor pool sizes
		uniformBlocksInPool = 70;
		texturesInPool = 70;
		setsInPool = 70;
		
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

		
		// Descriptor Layouts [what will be passed to the shaders]
		DSL.init(this, {
					// this array contains the bindings:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					//                  using the corresponding Vulkan constant
					// third  element : the pipeline stage where it will be used
					//                  using the corresponding Vulkan constant
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
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
				         sizeof(glm::vec2), UV}
				});

		// Pipelines [Shader couples]
		// The second parameter is the pointer to the vertex definition
		// Third and fourth parameters are respectively the vertex and fragment shaders
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		P.init(this, &VD, "shaders/ShaderVert.spv", "shaders/ShaderFrag.spv", {&DSL});

		// Models, textures and Descriptors (values assigned to the uniforms)

		// Create models
		// The second parameter is the pointer to the vertex definition for this model
		// The third parameter is the file name
		// The last is a constant specifying the file type: currently only OBJ or GLTF

        float u[2][3];
        float v[2][3];
        //black
        u[0][0] = 9.1f/11.0f;
        v[0][0] = 1.0f/11.0f;
        u[1][0]= 10.1f / 11.0f;
        v[1][0] = 2.0f / 11.0f;
        //white
        u[0][1] = 9.1f / 11.0f;
        v[0][1] = 9.0f / 11.0f;
        u[1][1]= 10.1f / 11.0f;
        v[1][1] = 10.0f / 11.0f;
        //brown
        u[0][2] = 8.1f / 11.0f;
        v[0][2] = 1.0f / 11.0f;
        u[1][2]= 9.1f / 11.0f;
        v[1][2] = 2.0f / 11.0f;

        MCB.vertices = {
                //Top
                {{-0.5f, -0.0001f, -0.5f}, {u[0][2], v[0][2]}},
                {{-0.5f, -0.0001f, 8.5f}, {u[0][2], v[1][2]}},
                {{8.5f, -0.0001f, 8.5f}, {u[1][2], v[1][2]}},
                {{8.5f, -0.0001f, -0.5f}, {u[1][2], v[0][2]}},
                // Bottom
                {{-0.5f, -1.0f, -0.5f}, {u[0][2], v[0][2]}},
                {{-0.5f, -1.0f, 8.5f}, {u[0][2], v[1][2]}},
                {{8.5f, -1.0f, 8.5f}, {u[1][2], v[1][2]}},
                {{8.5f, -1.0f, -0.5f}, {u[1][2], v[0][2]}}


        };
        MCB.indices = {
                // Front face
                1,5,6,1,6,2,
                // Back face
                0,7,4,0,3,7,
                // Top face
                0,1,2,0,2,3,
                // Bottom face
                4,6,5,4,7,6,
                // Left face
                0,4,5,0,5,1,
                // Right face
                2,6,7,2,7,3
        };
        MCB.initMesh(this, &VD);

        /*M1.vertices = {
                {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
                {{0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                {{1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                {{1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        };
        M1.indices = { 0,1,2,0,2,3};

        M1.initMesh(this, &VD);*/

        int c=1;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){

                MC[i][j].vertices = {
                        {{0.0f+(float)j, 0.0f, 0.0f+(float)i}, {u[0][c], v[0][c]}},
                        {{0.0f+(float)j, 0.0f, 1.0f+(float)i}, {u[0][c], v[1][c]}},
                        {{1.0f+(float)j, 0.0f, 1.0f+(float)i}, {u[1][c], v[1][c]}},
                        {{1.0f+(float)j, 0.0f, 0.0f+(float)i}, {u[1][c], v[0][c]}},
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
            }
        }

		// Create the textures
		// The second parameter is the file name
		T1.init(this,   "textures/Checker.png");
		T2.init(this,   "textures/Textures.png");
		
		// Init local variables
	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// This creates a new pipeline (with the current surface), using its shaders
		P.create();

		// Here you define the data set

        DSCB.init(this, &DSL, {
                {0, UNIFORM, sizeof(UniformBlock), nullptr},
                {1, TEXTURE, 0, &T2}
        });
        /*DS1.init(this, &DSL, {
                {0, UNIFORM, sizeof(UniformBlock), nullptr},
                {1, TEXTURE, 0, &T2}
        });*/

        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                DSC[i][j].init(this, &DSL, {
                        {0, UNIFORM, sizeof(UniformBlock), nullptr},
                        {1, TEXTURE, 0, &T2}
                });
            }
        }
	}

	// Here you destroy your pipelines and Descriptor Sets!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	void pipelinesAndDescriptorSetsCleanup() {
		// Cleanup pipelines
		P.cleanup();

		// Cleanup datasets

        DSCB.cleanup();
        //DS1.cleanup();
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                DSC[i][j].cleanup();
            }
        }
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// All the object classes defined in Starter.hpp have a method .cleanup() for this purpose
	// You also have to destroy the pipelines: since they need to be rebuilt, they have two different
	// methods: .cleanup() recreates them, while .destroy() delete them completely
	void localCleanup() {
		// Cleanup textures
		T1.cleanup();
		T2.cleanup();
		
		// Cleanup models

        MCB.cleanup();
        //M1.cleanup();
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                MC[i][j].cleanup();
            }
        }
		
		// Cleanup descriptor set layouts
		DSL.cleanup();
		
		// Destroies the pipelines
		P.destroy();		
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	
	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		// binds the pipeline
		P.bind(commandBuffer);
		// For a pipeline object, this command binds the corresponing pipeline to the command buffer passed in its parameter

		// binds the data set

        DSCB.bind(commandBuffer, P, 0, currentImage);
        MCB.bind(commandBuffer);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(MCB.indices.size()), 1, 0, 0, 0);
        /*DS1.bind(commandBuffer, P, 0, currentImage);
        M1.bind(commandBuffer);
        vkCmdDrawIndexed(commandBuffer,
                         static_cast<uint32_t>(M1.indices.size()), 1, 0, 0, 0);*/
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                DSC[i][j].bind(commandBuffer, P, 0, currentImage);
                MC[i][j].bind(commandBuffer);
                vkCmdDrawIndexed(commandBuffer,
                                 static_cast<uint32_t>(MC[i][j].indices.size()), 1, 0, 0, 0);
            }
        }

	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
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

        const float ROT_SPEED = glm::radians(120.0f);
        const float MOVE_SPEED = 2.0f;

        CamAlpha = CamAlpha - ROT_SPEED * deltaT * r.y;
        CamBeta  = CamBeta  - ROT_SPEED * deltaT * r.x;
        CamBeta  =  CamBeta < glm::radians(-90.0f) ? glm::radians(-90.0f) :
                    (CamBeta > glm::radians( 90.0f) ? glm::radians( 90.0f) : CamBeta);

        glm::vec3 ux = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(1,0,0,1);
        glm::vec3 uz = glm::rotate(glm::mat4(1.0f), CamAlpha, glm::vec3(0,1,0)) * glm::vec4(0,0,-1,1);
        CamPos = CamPos + MOVE_SPEED * m.x * ux * deltaT;
        CamPos = CamPos + MOVE_SPEED * m.y * glm::vec3(0,1,0) * deltaT;
        CamPos = CamPos + MOVE_SPEED * m.z * uz * deltaT;


		glm::mat4 World;
        glm::mat4 M = glm::perspective(glm::radians(45.0f), Ar, 0.1f, 50.0f);
        M[1][1] *= -1;

        glm::mat4 Mv =  glm::rotate(glm::mat4(1.0), -CamBeta, glm::vec3(1,0,0)) *
                        glm::rotate(glm::mat4(1.0), -CamAlpha, glm::vec3(0,1,0)) *
                        glm::translate(glm::mat4(1.0), -CamPos);

        glm::mat4 ViewPrj =  M * Mv;


        World = glm::translate(glm::mat4(1), glm::vec3(-2, -1, -2));
        uboCB.mvpMat = ViewPrj * World;
        DSCB.map(currentImage, &uboCB, sizeof(uboCB), 0);
        /*World = glm::translate(glm::mat4(1), glm::vec3(-2, -1, -2));
        ubo1.mvpMat = ViewPrj * World;
        DS1.map(currentImage, &ubo1, sizeof(ubo1), 0);*/
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                World = glm::translate(glm::mat4(1), glm::vec3(-2, -1, -2));
                uboC[i][j].mvpMat = ViewPrj * World;
                DSC[i][j].map(currentImage, &uboC[i][j], sizeof(uboC[i][j]), 0);
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