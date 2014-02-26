#version 130

uniform vec4 ambient;
uniform vec3 light1Color;
uniform vec3 light1Dir;
uniform vec3 hVector1;
uniform vec3 light2Color;
uniform vec3 light2Dir;
uniform vec3 hVector2;

in vec4 pass_color;
in vec3 in_position;

out vec4 out_color;

void main()
{
	float diff1 = max(0.0, dot(in_position, normalize(light1Dir)));
	float spec1 = max(0.0, dot(in_position, normalize(hVector1)));
	float diff2 = max(0.0, dot(in_position, normalize(light2Dir)));
	float spec2 = max(0.0, dot(in_position, normalize(hVector2)));

	if (diff1 == 0.0)
	{
		spec1 = 0.0;
	}
	else
	{
		spec1 = pow(spec1, 10);
	}
	if (diff2 == 0.0)
	{
		spec2 = 0.0;
	}
	else
	{
		spec2 = pow(spec2, 10);
	}
	
	vec3 amb = vec3(ambient);
	
	vec3 scattered = ambient.rgb + (light1Color * diff1) + (light2Color * diff2);
	vec3 reflected = light1Color * spec1 * 10;
	vec3 reflected2 = light2Color * spec2 * 15;

	vec3 finalColor = min(( pass_color.rgb * amb * scattered) + reflected + reflected2, vec3(1.0));

	out_color = vec4(finalColor, pass_color.a);
}
