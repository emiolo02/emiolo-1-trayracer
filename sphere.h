#pragma once
#include "object.h"
#include <stdlib.h>
#include <time.h>
#include "mat4.h"
#include "pbr.h"
#include "random.h"
#include "ray.h"
#include "material.h"
#include <iostream>

// returns a random point on the surface of a unit sphere
inline vec3 random_point_on_unit_sphere()
{
    float x = RandomFloatNTP();
    float y = RandomFloatNTP();
    float z = RandomFloatNTP();
    vec3 v( x, y, z );
    return normalize(v);
}

// a spherical object
class Sphere : public Object
{
public:
    float radius;
    vec3 center;
    Material material;

    Sphere(float radius, vec3 center, Material material) : 
        radius(radius),
        center(center),
        material(material)
    {

    }

    ~Sphere() override
    {
    
    }

    Color GetColor()
    {
        return material.color;
    }

    HitResult Intersect(Ray ray, float maxDist) override
    {
        /*
        */
        vec3 oc = ray.b - this->center;
        vec3 dir = ray.m;
        float b = dot(oc, dir);

        // early out if sphere is "behind" ray
        if (b > 0)
            return HitResult();

        float a = dot(dir, dir);
        float c = dot(oc, oc) - this->radius * this->radius;
        float discriminant = b * b - a * c;

        if (discriminant < 0)
            return HitResult();

        HitResult hit;

        constexpr float minDist = 0.001f;
        float sqrtDisc = sqrt(discriminant);
        float temp = (-b - sqrtDisc) / a;
        float temp2 = (-b + sqrtDisc) / a;

        if (temp < maxDist /*&& temp > minDist*/)
        {
            vec3 p = ray.PointAt(temp);
            hit.p = p;
            hit.normal = (p - this->center) * (1.0f / this->radius);
            hit.t = temp;
            hit.object = this;
            return HitResult(hit);
        }
        else if (temp2 < maxDist /*&& temp2 > minDist*/)
        {
            vec3 p = ray.PointAt(temp2);
            hit.p = p;
            hit.normal = (p - this->center) * (1.0f / this->radius);
            hit.t = temp2;
            hit.object = this;
            return HitResult(hit);
        }
        else
            return HitResult();

        /*
        vec3 origin = ray.b - center;
        vec3 rayDir = ray.m;

        float a = dot(rayDir, rayDir);
        float b = dot(origin, rayDir);
        if (b > 0)
            return Optional<HitResult>();

        float c = dot(origin, origin) - radius * radius;

        float disc = b * b - a * c;
        if (disc < 0)
            return Optional<HitResult>();

        float sqrtDisc = sqrt(disc);
        float temp = (-b - sqrtDisc)/a;
        float temp2 = (-b + sqrtDisc)/a;
        float minDist = 0.001;

        float t = 0;

        if (temp < maxDist && temp > minDist)
        {
            t = temp;
        }
        if (temp2 < maxDist && temp2 > minDist)
        {
            t = temp2;
        }
        
        HitResult hit;
        hit.p = origin + rayDir * t;
        hit.normal = normalize(center - hit.p);
        hit.object = this;
        hit.t = t;

        return hit;
        */
    }

    Ray ScatterRay(Ray ray, vec3 point, vec3 normal) override
    {
        return BSDF(this->material, ray, point, normal);
    }

};