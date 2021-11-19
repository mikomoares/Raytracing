#ifndef cylinder_H
#define cylinder_H

#include "hittable.h"
#include "vec3.h"

class cylinder : public hittable {
   public:
       cylinder() {}
       cylinder(point3 cen, double r, double h, shared_ptr<material> m)
           : center(cen), radius(r), height(h), mat_ptr(m) {};

       virtual bool hit(
           const ray& r, double t_min, double t_max, hit_record& rec) const override;

   public:
       point3 center;
       double radius;
       double height;
       shared_ptr<material> mat_ptr;
};

bool cylinder::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    double xe = r.origin()[0];
    double ye = r.origin()[1];
    double ze = r.origin()[2];
    double xd = r.direction()[0];
    double yd = r.direction()[1];
    double zd = r.direction()[2];
    
    double y_min = center[1];
    double y_max = y_min + height;

    auto a = xd*xd + zd*zd;
    auto b = 2*xe*xd + 2*ze*zd;
    auto c = xe*xe + ze*ze - 1;

    vec3 oc = r.origin() - center;

    auto discriminant = b*b - 4*a*c;
 
    if (discriminant < 0) return false;

    auto sqrtd = sqrt(discriminant);

    double t1;
    double t2;
    double t3;
    double t4;
    double t;

    t1 = (-b - sqrtd) / (2*a);
    t2 = (-b + sqrtd) / (2*a);
    t3 = (y_max - ye) / yd; 
    t4 = (y_min - ye) / yd; 
    
    auto y1 = ye + t1 * yd;
    auto y2 = ye + t2 * yd;

    t=t1;
    if (t1 < t_min || t_max < t1) {
        t=t2;
        if (t2 < t_min || t_max < t2){
            return false;
        } 
    }

    if((y1 > y_min) && (y2 > y_min) && (y1 < y_max) && (y2 < y_max)){
        t=t2;
    } 
    else if(!(y1 > y_min) != !(y2 > y_min)){
        t=t3;
    }
    else if(!(y1 < y_max) != !(y2 < y_max)){
        t=t4;
    }else return false;

    rec.t = t;
    rec.p = r.at(rec.t);
    
    vec3 norm = vec3(center[0], center[1], rec.p[2]);
    vec3 outward_normal = (rec.p - norm) / radius;
    if (t == t3){
        vec3 outward_normal = vec3(0, 0, 1);
    }else if (t == t4){
        vec3 outward_normal = vec3(0, 0, -1);
    }
    
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    
    return true;
}

#endif