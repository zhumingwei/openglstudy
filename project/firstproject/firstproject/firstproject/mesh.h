#pragma once
#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_s.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex
{
	//position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	//texCoords
	glm::vec2 TexCoords;
	//tangent
	glm::vec3 Tangent;
	//bitangent;
	glm::vec3 BiTangent;
	
};

struct Texture {
	unsigned int id;
	string type;
};

struct Mesh
{
public:
	/*��������*/
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	/*����*/
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setupMesh();
	}
	void Draw(Shader shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);//�ڰ�֮ǰ������Ӧ������Ԫ
			//��ȡ������� (diffuse_textureN �е� N)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr ++);
			}
			else if (name == "texture_specular")
				number = std::to_string(specularNr ++);

			shader.setFloat(("material." + name + number).c_str, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);
		//������·
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
private:
	/*��Ⱦ����*/
	unsigned int VAO, VBO, EBO;
	/*����*/
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		//����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		//������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}
};




#endif // !MESH_H
