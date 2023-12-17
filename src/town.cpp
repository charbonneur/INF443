#include "town.hpp"
#include <string> 
using namespace cgp;
using namespace std;
float taille = 5.0f;
//geometrie de forme
mesh evoluer(mesh base, string type) {
	mesh evolution;
	
	buffer<vec3> pp = base.position;
	vec3 normal = 0.01*base.normal[0];
	
	if (type == "sol") {
		evolution.push_back(base);
		//construire le batiment: immeuble
		
		float rate=(rand()+0.0f)/RAND_MAX;
		cout << rate << endl;
		if (rate<0.6) {
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[0], pp[1], pp[1] + vec3(0.0, 0.0, taille), pp[0] + vec3(0.0, 0.0, taille)), "mur"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[1], pp[2], pp[2] + vec3(0.0, 0.0, taille), pp[1] + vec3(0.0, 0.0, taille)), "mur"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[2], pp[3], pp[3] + vec3(0.0, 0.0, taille), pp[2] + vec3(0.0, 0.0, taille)), "mur"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[3], pp[0], pp[0] + vec3(0.0, 0.0, taille), pp[3] + vec3(0.0, 0.0, taille)), "mur"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[0] + vec3(0.0, 0.0, taille), pp[1] + vec3(0.0, 0.0, taille), pp[2] + vec3(0.0, 0.0, taille), pp[3] + vec3(0.0, 0.0, taille)), "toit"));
		}
		//construire le batiment : tour
		else {
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[0], pp[1], pp[1] + vec3(0.0, 0.0,3* taille), pp[0] + vec3(0.0, 0.0,3* taille)), "mur_tour"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[1], pp[2], pp[2] + vec3(0.0, 0.0, 3*taille), pp[1] + vec3(0.0, 0.0,3* taille)), "mur_tour"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[2], pp[3], pp[3] + vec3(0.0, 0.0, 3*taille), pp[2] + vec3(0.0, 0.0,3* taille)), "mur_tour"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[3], pp[0], pp[0] + vec3(0.0, 0.0,3* taille), pp[3] + vec3(0.0, 0.0, 3*taille)), "mur_tour"));
			evolution.push_back(evoluer(mesh_primitive_quadrangle(pp[0] + vec3(0.0, 0.0, 3*taille), pp[1] + vec3(0.0, 0.0, 3*taille), pp[2] + vec3(0.0, 0.0,3* taille), pp[3] + vec3(0.0, 0.0, 3*taille)), "toit"));
		}
		
	}
	if (type == "mur") {
		base.color.fill({ 0.866, 0.596, 0.360 });
		evolution.push_back(base);
		//faire des fenetre
		buffer<vec3> ppn = pp + normal;
		//premier colonne
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 8 + normalize(ppn[3] - ppn[0]) * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 8 + normalize(ppn[3] - ppn[0]) * 7 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 24 + normalize(ppn[3] - ppn[0]) * 7 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 24 + normalize(ppn[3] - ppn[0]) * taille / 8), "fenetre"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 8 + normalize(ppn[3] - ppn[0]) * 5*taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 8 + normalize(ppn[3] - ppn[0]) * 7 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 24 + normalize(ppn[3] - ppn[0]) * 7 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 24 + normalize(ppn[3] - ppn[0]) * 5*taille / 12), "fenetre"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 8 + normalize(ppn[3] - ppn[0]) * 17*taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 8 + normalize(ppn[3] - ppn[0]) * 7 * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 24 + normalize(ppn[3] - ppn[0]) * 7 * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 24 + normalize(ppn[3] - ppn[0]) * 17*taille / 24), "fenetre"));
		
		//deuxieme colonne
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 5*taille /12 + normalize(ppn[3] - ppn[0]) * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 5*taille / 12 + normalize(ppn[3] - ppn[0]) * 7 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 12 + normalize(ppn[3] - ppn[0]) * 7 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 12 + normalize(ppn[3] - ppn[0]) * taille / 8), "fenetre"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 5 * taille / 12 + normalize(ppn[3] - ppn[0]) * 5 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 5 * taille / 12 + normalize(ppn[3] - ppn[0]) * 7 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 12 + normalize(ppn[3] - ppn[0]) * 7 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 12 + normalize(ppn[3] - ppn[0]) * 5 * taille / 12), "fenetre"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 5 * taille / 12 + normalize(ppn[3] - ppn[0]) * 17*taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 5 * taille / 12 + normalize(ppn[3] - ppn[0]) * 7 * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 12 + normalize(ppn[3] - ppn[0]) * 7 * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 12 + normalize(ppn[3] - ppn[0]) * 17*taille / 24), "fenetre"));

		//troisiemme colonne
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 17 * taille / 24 + normalize(ppn[3] - ppn[0]) * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 17 * taille / 24 + normalize(ppn[3] - ppn[0]) * 7 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 8 + normalize(ppn[3] - ppn[0]) * 7 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 8 + normalize(ppn[3] - ppn[0]) * taille / 8), "fenetre"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 17 * taille / 24 + normalize(ppn[3] - ppn[0]) * 5 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 17 * taille / 24 + normalize(ppn[3] - ppn[0]) * 7 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 8 + normalize(ppn[3] - ppn[0]) * 7 * taille / 12, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 8 + normalize(ppn[3] - ppn[0]) * 5 * taille / 12), "fenetre"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 17 * taille / 24 + normalize(ppn[3] - ppn[0]) * 17 * taille / 24, ppn[0] + normalize(ppn[1] - ppn[0]) * 17 * taille / 24 + normalize(ppn[3] - ppn[0]) * 7 * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 8 + normalize(ppn[3] - ppn[0]) * 7 * taille / 8, ppn[0] + normalize(ppn[1] - ppn[0]) * 7 * taille / 8 + normalize(ppn[3] - ppn[0]) * 17 * taille / 24), "fenetre"));
	}
	if (type == "mur_tour") {
		base.color.fill({ 0.866, 0.596, 0.360 });
		evolution.push_back(base);
		buffer<vec3> ppn = pp + normal;
		//porte
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) *  9*taille / 20 , ppn[0] + normalize(ppn[1] - ppn[0]) * 9 * taille / 20 + normalize(ppn[3] - ppn[0]) *  taille / 5, ppn[0] + normalize(ppn[1] - ppn[0]) * 11 * taille / 20 + normalize(ppn[3] - ppn[0]) *  taille / 5, ppn[0] + normalize(ppn[1] - ppn[0]) * 11* taille / 20 ), "porte"));
		//grosse fenetre
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * taille / 5 + normalize(ppn[3] - ppn[0]) * 11 * taille / 5, ppn[0] + normalize(ppn[1] - ppn[0]) * 1 * taille / 5 + normalize(ppn[3] - ppn[0]) * 14 * taille / 5, ppn[0] + normalize(ppn[1] - ppn[0]) * 4 * taille / 5 + normalize(ppn[3] - ppn[0]) * 14 * taille / 5, ppn[0] + normalize(ppn[1] - ppn[0]) * 4 * taille / 5 + normalize(ppn[3] - ppn[0]) * 11 * taille / 5), "fenetre"));
		
	}
	if (type == "fenetre") {
		buffer<vec3> ppn = pp - normal;
		base.color.fill({ 0.0,0.0,0.0 });
		float length = norm(pp[0] - pp[1]);
		evolution.push_back(base);
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] , ppn[0] + normalize(ppn[1] - ppn[0]) * 0.45* length , ppn[0] + normalize(ppn[1] - ppn[0]) *0.45*length + normalize(ppn[3] - ppn[0]) * 0.45 * length , ppn[0] + normalize(ppn[3] - ppn[0]) *0.45*length), "carreau"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 0.55 * length, ppn[1], ppn[1] +  normalize(ppn[3] - ppn[0]) * 0.45 * length, ppn[0] + normalize(ppn[3] - ppn[0]) * 0.45 * length+ normalize(ppn[1] - ppn[0]) * 0.55 *length), "carreau"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[3] - ppn[0]) * 0.55 * length, ppn[0] + normalize(ppn[1] - ppn[0]) * 0.45 * length + normalize(ppn[3] - ppn[0]) * 0.55 * length, ppn[3] + normalize(ppn[1] - ppn[0]) * 0.45 * length , ppn[3]), "carreau"));
		evolution.push_back(evoluer(mesh_primitive_quadrangle(ppn[0] + normalize(ppn[1] - ppn[0]) * 0.55 * length + normalize(ppn[3] - ppn[0]) * 0.55 * length, ppn[1] + normalize(ppn[3] - ppn[0]) * 0.55 * length, ppn[2], ppn[3] + normalize(ppn[1] - ppn[0]) * 0.55*length), "carreau"));
		

	}
	if (type == "porte") {
		base.color.fill({ 0.345, 0.160, 0 });
		evolution.push_back(base);
	}
	if (type == "toit") {
		base.color.fill({ 0.866, 0.596, 0.360 });
		evolution.push_back(base);
		
		float rate = (rand()+0.0f) / RAND_MAX;
		if (rate < 0.5) {
			vec3 center = vec3{0.0,0.0,taille/10}+(pp[0] + pp[2]) / 2;
			evolution.push_back(evoluer(mesh_primitive_triangle(pp[0], pp[1], center), "toiture"));
			evolution.push_back(evoluer(mesh_primitive_triangle(pp[1], pp[2], center), "toiture"));
			evolution.push_back(evoluer(mesh_primitive_triangle(pp[2], pp[3], center), "toiture"));
			evolution.push_back(evoluer(mesh_primitive_triangle(pp[3], pp[0], center), "toiture"));
		}
	}
	if (type == "toiture") {
		
		base.color[0] = { 0, 0.337, 0.105 };
		evolution.push_back(base);
	}
	if (type == "carreau") {

		base.color[0] = { 0.372, 0.619, 0.627 };
		evolution.push_back(base);
	}
	else {
		return evolution;
	}
	return evolution;
}
//construit un batiment selon les regles etablies
mesh construire() {
	float s = taille / 2;
	mesh sol = mesh_primitive_quadrangle(vec3{ -s,-s,0.0 }, vec3{ s,-s,0.0 }, vec3{ s,s,0.0 }, vec3{ -s,s,0.0 });
	return evoluer(sol, "sol");
}
//construit une ville de 9 batiments sous forme de 3x3 grâce à des translations et construire()
mesh town() {
	mesh town;
	mesh building;
	float translation = taille * 6 / 5;
	for (int i = 0; i < 9; i++) {
		building = construire();
		for (auto& x : building.position) {
			if (i % 3 == 0) {
				x += vec3{- translation,0.0f,0.0f };
			}
			if (i % 3 == 2) {
				x += vec3{ translation,0.0f,0.0f };
			}
			if (i <3) {
				x += vec3{ 0.0f,-translation,0.0f };
			}
			if (i >5) {
				x += vec3{ 0.0f,translation,0.0f };
			}
		}
		town.push_back(building);
	}
	return town;
}
