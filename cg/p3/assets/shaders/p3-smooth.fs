#version 330 core

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 spot;
	float shine;
};

struct Luz
{
	vec4 cor;
	vec3 posicao;
	vec3 direcao;
	int fator;
	float angulo_interno, angulo_externo;
	int tipo;
};

uniform int flatMode;
uniform vec3 view_position;
uniform Material material;
uniform Luz luz[8];
uniform int quantidade_luz;
uniform vec4 ambientLight;

in vec3 Normal;
in vec4 P;

out vec4 fragmentColor;

void main()
{
	vec3 Visao = normalize(view_position - P.xyz);

	fragmentColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for(int i = 0; i < quantidade_luz; i++)
	{
		if(luz[i].tipo == 0)
		{
			vec3 Luz = normalize(-luz[i].direcao);
			vec3 Reflexao = reflect(-Luz, Normal);
			vec4 Ambient = ambientLight * luz[i].cor * float(1 - flatMode) * material.ambient;
			vec4 Diffuse = material.diffuse * luz[i].cor * max(dot(Normal, Luz), float(flatMode));
			vec4 Specular = material.spot * luz[i].cor * pow(max(dot(Reflexao, Visao), 0), material.shine);
			fragmentColor = fragmentColor + (Ambient + Diffuse + Specular);
		}
		else if(luz[i].tipo == 1)
		{
			vec3 Luz = normalize(luz[i].posicao - vec3(P));
			vec3 Reflexao = reflect(-Luz, Normal);
			vec4 Ambient = ambientLight * luz[i].cor * float(1 - flatMode) * material.ambient;
			vec4 Diffuse = material.diffuse * luz[i].cor * max(dot(Normal, Luz), float(flatMode));
			vec4 Specular = material.spot * luz[i].cor * pow(max(dot(Reflexao, Visao), 0), material.shine);		
			float cai = length(luz[i].posicao - vec3(P));
			fragmentColor = fragmentColor + (Ambient + Diffuse + Specular) / pow(cai, luz[i].fator);
		}
		else if(luz[i].tipo == 2)
		{
			vec3 Luz = normalize(luz[i].posicao - vec3(P));
			float theta = dot(normalize(-luz[i].direcao), Luz);
			float epsilon = luz[i].angulo_interno - luz[i].angulo_externo;
			float intensidade = clamp((theta - luz[i].angulo_externo) / epsilon, 0.0, 1.0);
			vec3 Reflexao = reflect(-Luz, Normal);
			vec4 Ambient = ambientLight * luz[i].cor * float(1 - flatMode) * material.ambient;
			vec4 Diffuse = material.diffuse * luz[i].cor * max(dot(Normal, Luz), float(flatMode));
			vec4 Specular = material.spot * luz[i].cor * pow(max(dot(Reflexao, Visao), 0), material.shine);
			float cai = length(luz[i].posicao - vec3(P));
			fragmentColor = fragmentColor + intensidade * (Ambient + Diffuse + Specular) / pow(cai, luz[i].fator);
		}		
	}
}