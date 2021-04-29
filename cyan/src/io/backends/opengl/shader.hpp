#pragma once


#include <string>

#include <GL/glew.h>


namespace cyan::io::backends::opengl {
    enum class ShaderType {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
    };

    /**
     * Wraps an OpenGL shader program.
     */
    struct Shader {
        /**
         * Default-construct an unusable shader.
         */
        Shader();

        /**
         * Move constructor.
         */
        Shader(Shader&&) noexcept;

        /// Delete copy-constructors.
        Shader(Shader&) = delete;
        Shader operator=(Shader&) = delete;

        /**
         * Compile and link a shader from paths to a vertex and fragment shader.
         * @param vert_path The location of the file containing the vertex shader source.
         * @param frag_path The location of the file containing the fragment shader source.
         * @throws cyan::Error on failure to compile or link.
         */
        Shader(const std::string& vert_path, const std::string& frag_path);

        /**
         * Move assignment.
         */
        Shader& operator=(Shader&&) noexcept;

        /// Destructor to delete the wrapped shader program.
        ~Shader();

        /// Use the wrapped shader.
        void use();

    private:
        unsigned int shader_program;
    };

    /**
     * Compile an OpenGL shader from source, and return the handle to the compiled shader.
     * @param shader_type The type of the shader to compile (either ShaderType::Vertex or ShaderType::Fragment)
     * @param shader_src The shader source
     * @returns An (unsigned int) handle to the compiled shader.
     * @throws cyan::Error on failure to compile.
     */
    unsigned int compile_shader(ShaderType shader_type, const std::string& shader_src);

    /**
     * Link a compiled vertex and fragment shaders into a shader program.
     * @param vertex_shader A handle to a compiled vertex shader
     * @param fragment_shader A handle to a compiled fragment shader
     * @returns An (unsigned int) handle to the linked shader program.
     * @throws cyan::Error on failure to link the shaders.
     */
    unsigned int link_shaders(unsigned int vertex_shader, unsigned int fragment_shader);
}
