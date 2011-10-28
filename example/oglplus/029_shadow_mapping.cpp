/**
 *  @example oglplus/029_shadow_mapping.cpp
 *  @brief Shows how to do dynamic shadow mapping
 *
 *  @image html 029_shadow_mapping.png
 *
 *  Copyright 2008-2011 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#include <oglplus/gl.hpp>
#include <oglplus/all.hpp>
#include <oglplus/shapes/cube.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>

#include <sstream>

#include "example.hpp"

namespace oglplus {

class ShadowMapExample : public Example
{
private:
	// helper object building cube vertex attributes
	shapes::Cube make_cube;
	// helper object encapsulating cube drawing instructions
	shapes::DrawingInstructions cube_instr;
	// indices pointing to cube primitive elements
	shapes::Cube::IndexArray cube_indices;

	// The offsets of the individual instances of the cube
	std::vector<Vec3f> cube_offsets;

	// Returns a vector of cube offsets
	static std::vector<Vec3f> MakeCubeOffsets(
		GLfloat distance,
		GLint cubes_per_side
	)
	{
		GLint instance_count = cubes_per_side * cubes_per_side;
		std::vector<Vec3f> offsets(instance_count);

		auto i = offsets.begin(), e = offsets.end();

		float d = distance * cubes_per_side;

		for(GLint z=0; z!=cubes_per_side; ++z)
		for(GLint x=0; x!=cubes_per_side; ++x)
		{
			assert(i != e);
			*i = Vec3f(
				(GLfloat(x)/(cubes_per_side-1) - 0.5f)*d,
				0.5f,
				(GLfloat(z)/(cubes_per_side-1) - 0.5f)*d
			);
			++i;
		}
		assert(i == e);

		return offsets;
	}

	// wrapper around the current OpenGL context
	Context gl;

	// Shaders
	VertexShader vs;
	FragmentShader draw_fs, shadow_fs;

	// Programs and pipelines
	Program vert_prog, draw_prog, shadow_prog;
	ProgramPipeline draw_pp, shadow_pp;

	// A vertex array object for the rendered plane and cubes
	VertexArray plane, cube;

	// VBOs for the objects' vertex attributes
	Buffer plane_verts, plane_normals;
	Buffer cube_verts, cube_normals;


	// The resolution of the texture (side x side)
	size_t tex_side;

	// The paths of the lights
	const std::vector<CubicBezierLoop<Vec3f, double> > light_paths;

	// Here the current positions of lights will be stored
	std::vector<Vec3f> light_positions;
	// Here the shadow texture projection matrices will be stored
	std::vector<Matrix4f> light_proj_matrices;

	// The colors of the lights
	const std::vector<Vec3f> light_colors;

	// The depth map array
	const Array<Texture> smap;

	// The framebuffers used for rendering into the texture
	const Array<Framebuffer> fbo;

	// The viewport width and height
	size_t width, height;
public:
	ShadowMapExample(void)
	 : cube_instr(make_cube.Instructions())
	 , cube_indices(make_cube.Indices())
	 , cube_offsets(MakeCubeOffsets(1.8f, 7))
	 , vs("Vertex")
	 , draw_fs("Draw fragment")
	 , shadow_fs("Shadow fragment")
	 , vert_prog("Vertex")
	 , draw_prog("Draw")
	 , shadow_prog("Shadow")
	 , draw_pp("Draw")
	 , shadow_pp("Shadow")
	 , tex_side(512)
	 , light_paths(
		{
			CubicBezierLoop<Vec3f, double>({
				{-9.0f,  5.0f,  8.0f},
				{ 0.0f,  6.0f, -9.0f},
				{ 9.0f,  5.0f,  9.0f},
				{ 0.0f, 15.0f,  0.0f}
			}),
			CubicBezierLoop<Vec3f, double>({
				{-8.0f,  5.0f,  9.0f},
				{-9.0f,  9.0f, -9.0f},
				{ 9.0f,  8.0f, -9.0f},
				{ 9.0f,  4.0f,  9.0f}
			}),
			CubicBezierLoop<Vec3f, double>({
				{-9.0f,  7.0f,  9.0f},
				{ 9.0f,  5.0f,  9.0f},
				{ 9.0f,  5.0f, -9.0f},
				{-9.0f,  8.0f, -9.0f}
			})
		}
	), light_positions(light_paths.size())
	 , light_proj_matrices(light_paths.size())
	 , light_colors(
		{
			{1.0f, 0.1f, 0.01},
			{0.1f, 1.0f, 0.1f},
			{0.1f, 0.1f, 1.0f}
		}
	), smap(light_paths.size())
	 , fbo(light_paths.size())
	{
		assert(light_paths.size() == light_positions.size());
		assert(light_paths.size() == light_colors.size());

		std::stringstream max_lights_def, cube_count_def;
		max_lights_def <<"#define MaxLights "<<light_paths.size()<<'\n';
		cube_count_def <<"#define CubeCount "<<cube_offsets.size()<<'\n';
		// Set the vertex shader source
		vs.Source({
			"#version 330\n",
			max_lights_def.str().c_str(),
			cube_count_def.str().c_str(),
			"uniform mat4 CameraMatrix, ProjectionMatrix;"
			"uniform mat4 TexProjectionMatrices[MaxLights];"
			"uniform vec3 LightPositions[MaxLights];"
			"uniform vec3 Offsets[CubeCount];"
			"uniform int UseOffset;"
			"uniform int LightCount;"
			"in vec3 Position, Normal;"
			"out gl_PerVertex {"
			"	vec4 gl_Position;"
			"};"
			"out vec3 vertNormal;"
			"out vec3 vertLight[MaxLights];"
			"out vec4 vertTexCoord[MaxLights];"
			"void main(void)"
			"{"
			"	vec3 newPos = Position;"
			"	if(UseOffset != 0)"
			"		newPos += Offsets[gl_InstanceID];"
			"	gl_Position = vec4(newPos, 1.0);"
			"	vertNormal = Normal;"
			"	for(int i=0; i!=LightCount; ++i)"
			"	{"
			"		vertLight[i] = ("
			"			LightPositions[i] -"
			"			gl_Position.xyz"
			"		);"
			"		vertTexCoord[i] = "
			"			TexProjectionMatrices[i] *"
			"			gl_Position;"
			"	}"
			"	gl_Position ="
			"		ProjectionMatrix *"
			"		CameraMatrix *"
			"		gl_Position;"
			"}"
		});
		// compile it
		vs.Compile();

		vert_prog.AttachShader(vs);
		vert_prog.MakeSeparable();
		vert_prog.Link();
		vert_prog.Use();

		Uniform(vert_prog, "Offsets").Set(cube_offsets);

		// bind the VAO for the plane
		plane.Bind();

		// bind the VBO for the plane vertices
		plane_verts.Bind(Buffer::Target::Array);
		{
			GLfloat data[4*3] = {
				-100.0f, 0.0f,  100.0f,
				-100.0f, 0.0f, -100.0f,
				 100.0f, 0.0f,  100.0f,
				 100.0f, 0.0f, -100.0f
			};
			// upload the data
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			// setup the vertex attribs array for the vertices
			vert_prog.Use();
			VertexAttribArray attr(vert_prog, "Position");
			attr.Setup(3, DataType::Float);
			attr.Enable();
		}

		// bind the VBO for the plane normals
		plane_normals.Bind(Buffer::Target::Array);
		{
			GLfloat data[4*3] = {
				 0.0f, 1.0f,  0.0f,
				 0.0f, 1.0f,  0.0f,
				 0.0f, 1.0f,  0.0f,
				 0.0f, 1.0f,  0.0f
			};
			// upload the data
			Buffer::Data(Buffer::Target::Array, 4*3, data);
			// setup the vertex attribs array for the normals
			vert_prog.Use();
			VertexAttribArray attr(vert_prog, "Normal");
			attr.Setup(3, DataType::Float);
			attr.Enable();
		}

		// bind the VAO for the cube
		cube.Bind();

		// bind the VBO for the cube vertices
		cube_verts.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Positions(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			vert_prog.Use();
			VertexAttribArray cube_attr(vert_prog, "Position");
			cube_attr.Setup(n_per_vertex, DataType::Float);
			cube_attr.Enable();
		}

		// bind the VBO for the cube normals
		cube_normals.Bind(Buffer::Target::Array);
		{
			std::vector<GLfloat> data;
			GLuint n_per_vertex = make_cube.Normals(data);
			// upload the data
			Buffer::Data(Buffer::Target::Array, data);
			// setup the vertex attribs array for the vertices
			vert_prog.Use();
			VertexAttribArray cube_attr(vert_prog, "Normal");
			cube_attr.Setup(n_per_vertex, DataType::Float);
			cube_attr.Enable();
		}

		VertexArray::Unbind();

		// set the fragment shader source
		draw_fs.Source({
			"#version 330\n",
			max_lights_def.str().c_str(),
			"uniform vec3 LightColors[MaxLights];"
			"uniform sampler2DShadow ShadowTexs[MaxLights];"
			"in vec3 vertNormal;"
			"in vec3 vertLight[MaxLights];"
			"in vec4 vertTexCoord[MaxLights];"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	vec3 color = vec3(0.1, 0.1, 0.1);"
			"	for(int i=0; i!=MaxLights; ++i)"
			"	{"
			"		float d = dot("
			"			normalize(vertNormal),"
			"			normalize(vertLight[i])"
			"		);"
			"		if(d > 0.0)"
			"		{"
			"			float l = sqrt(length(vertLight[i]));"
			"			d /= l;"
			"			vec3 coord = ("
			"				vertTexCoord[i].xyz /"
			"				vertTexCoord[i].w"
			"			) * 0.5 + 0.5;"
			"			if("
			"				coord.x >= 0.0 && "
			"				coord.x <= 1.0 && "
			"				coord.y >= 0.0 && "
			"				coord.y <= 1.0 && "
			"				coord.z <= 1.0"
			"			)"
			"			{"
			"				d *= texture("
			"					ShadowTexs[i], "
			"					coord"
			"				);"
			"			}"
			"		}"
			"		color += LightColors[i] *"
			"			3.2 * max(d, 0.0);"
			"	}"
			"	fragColor = vec4(color, 1.0);"
			"}"
		});
		// compile it
		draw_fs.Compile();

		draw_prog.AttachShader(draw_fs);
		draw_prog.MakeSeparable();
		draw_prog.Link();

		ProgramUniform(draw_prog, "LightColors").Set(light_colors);

		draw_pp.Bind();
		draw_prog.Use();
		draw_pp.UseStages(vert_prog).Vertex();
		draw_pp.UseStages(draw_prog).Fragment();


		// set the fragment shader source
		shadow_fs.Source(
			"#version 330\n"
			"out vec4 fragColor;"
			"void main(void)"
			"{"
			"	fragColor = vec4(0.0, 0.0, 0.0, 1.0);"
			"}"
		);
		// compile it
		shadow_fs.Compile();

		shadow_prog.AttachShader(shadow_fs);
		shadow_prog.MakeSeparable();
		shadow_prog.Link();

		shadow_pp.Bind();
		shadow_prog.Use();
		shadow_pp.UseStages(vert_prog).Vertex();
		shadow_pp.UseStages(shadow_prog).Fragment();

		ProgramPipeline::Unbind();
		Program::UseNone();


		std::vector<GLint> tex_units(light_paths.size());
		for(GLint i=0, n=light_paths.size(); i!=n; ++i)
		{
			Texture::Active(i);
			tex_units[i] = i;

			auto bound_tex = Bind(smap[i], Texture::Target::_2D);
			bound_tex.MinFilter(TextureMinFilter::Linear);
			bound_tex.MagFilter(TextureMagFilter::Linear);
			bound_tex.WrapS(TextureWrap::ClampToEdge);
			bound_tex.WrapT(TextureWrap::ClampToEdge);
			bound_tex.CompareMode(
				TextureCompareMode::CompareRefToTexture
			);
			bound_tex.Image2D(
				0,
				PixelDataInternalFormat::DepthComponent32F,
				tex_side, tex_side,
				0,
				PixelDataFormat::DepthComponent,
				PixelDataType::Float,
				nullptr
			);

			auto bound_fbo = Bind(
				fbo[i],
				Framebuffer::Target::Draw
			);
			bound_fbo.AttachTexture(
				Framebuffer::Attachment::Depth,
				smap[i],
				0
			);
		}
		ProgramUniform(draw_prog, "ShadowTexs").Set<1>(tex_units);

		//
		gl.Enable(Capability::DepthTest);
		gl.Enable(Capability::CullFace);
		gl.CullFace(Face::Back);
		gl.PolygonOffset(1.0, 1.0);
	}

	void Reshape(size_t vp_width, size_t vp_height)
	{
		width = vp_width;
		height = vp_height;
	}

	void Render(double time)
	{
		assert(light_paths.size() == light_positions.size());

		shadow_pp.Bind();

		const auto light_persp_matrix = CamMatrixf::Perspective(
			Degrees(48),
			1.0, 1.0, 20.0
		);
		ProgramUniform(vert_prog, "LightCount").Set(0);
		ProgramUniform(vert_prog, "ProjectionMatrix").SetMatrix(
			light_persp_matrix
		);

		gl.Enable(Capability::PolygonOffsetFill);

		for(GLint i=0, n=light_positions.size(); i!=n; ++i)
		{
			light_positions[i] =
				light_paths[i].Position((i+1)*time/12.0);

			const auto light_matrix = CamMatrixf::LookingAt(
				light_positions[i],
				Vec3f()
			);

			light_proj_matrices[i] =
				light_persp_matrix *
				light_matrix;

			// Bind the off-screen FBO
			fbo[i].Bind(Framebuffer::Target::Draw);
			// Set the viewport size and clear it
			gl.Viewport(tex_side, tex_side);
			gl.ClearDepth(1.0f);
			gl.Clear().DepthBuffer();

			ProgramUniform(vert_prog, "CameraMatrix").SetMatrix(
				light_matrix
			);

			gl.FrontFace(make_cube.FaceWinding());
			ProgramUniform(vert_prog, "UseOffset").Set(1);
			cube.Bind();
			cube_instr.Draw(cube_indices, cube_offsets.size());

			gl.FrontFace(FaceOrientation::CW);
			ProgramUniform(vert_prog, "UseOffset").Set(0);
			plane.Bind();
			gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
		}

		gl.Disable(Capability::PolygonOffsetFill);

		// Now we're going to draw into the default framebuffer
		Framebuffer::BindDefault(Framebuffer::Target::Draw);

		ProgramUniform(vert_prog, "LightPositions").Set(light_positions);

		ProgramUniform(vert_prog, "TexProjectionMatrices").SetMatrix(
			light_proj_matrices
		);

		ProgramUniform(vert_prog, "LightCount").Set(
			int(light_positions.size())
		);

		ProgramUniform(vert_prog, "ProjectionMatrix").SetMatrix(
			CamMatrixf::Perspective(
				Degrees(50),
				double(width)/height,
				1, 100
			)
		);
		ProgramUniform(vert_prog, "CameraMatrix").SetMatrix(
			CamMatrixf::Orbiting(
				Vec3f(),
				25.0 + SineWave(time / 20.0) * 10.0,
				FullCircles(time / 8.0),
				Degrees(50 + SineWave(time / 15.0) * 30)
			)
		);

		// Set the viewport and perspective matrix
		gl.Viewport(width, height);
		// clear it
		gl.ClearColor(0.1f, 0.1f, 0.1f, 0.0f);
		gl.ClearDepth(1.0f);
		gl.Clear().ColorBuffer().DepthBuffer();

		gl.PolygonMode(Face::Front, PolygonMode::Line);
		gl.FrontFace(make_cube.FaceWinding());
		ProgramUniform(vert_prog, "UseOffset").Set(1);
		cube.Bind();
		cube_instr.Draw(cube_indices, cube_offsets.size());

		draw_pp.Bind();

		gl.PolygonMode(Face::Front, PolygonMode::Fill);
		gl.FrontFace(make_cube.FaceWinding());
		ProgramUniform(vert_prog, "UseOffset").Set(1);
		cube.Bind();
		cube_instr.Draw(cube_indices, cube_offsets.size());

		gl.FrontFace(FaceOrientation::CW);
		ProgramUniform(vert_prog, "UseOffset").Set(0);
		plane.Bind();
		gl.DrawArrays(PrimitiveType::TriangleStrip, 0, 4);
	}

	bool Continue(double time)
	{
		return time < 60.0;
	}
};

std::unique_ptr<Example> makeExample(void)
{
	return std::unique_ptr<Example>(new ShadowMapExample);
}

} // namespace oglplus
