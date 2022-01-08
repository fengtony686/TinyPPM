#include <cstdio>
#include "utils/utils.h"
#include "monitors/manager.h"

const bool ANTI_ALIASING = false;
const double focalLength = 0;
const double aperture = 2.5;
const int width = 512, height = 384;

int main() {
    const double bezier_div_x = 3;
    const double bezier_div_y = 2.5;
    std::vector<double> control_x = {20. / bezier_div_x, 27. / bezier_div_x, 30. / bezier_div_x, 30. / bezier_div_x,
                                     30. / bezier_div_x, 25. / bezier_div_x, 20. / bezier_div_x, 15. / bezier_div_x,
                                     30. / bezier_div_x};
    std::vector<double> control_y = {0. / bezier_div_y, 0. / bezier_div_y, 10. / bezier_div_y, 20. / bezier_div_y,
                                     30. / bezier_div_y, 40. / bezier_div_y, 60. / bezier_div_y, 70. / bezier_div_y,
                                     80. / bezier_div_y};
    Bezier2D bezier(control_x, control_y, 9, .365);

    std::vector<double> water_x = {0. / bezier_div_x, 40. / bezier_div_x, 80. / bezier_div_x, 0. / bezier_div_x};
    std::vector<double> water_y = {0. / bezier_div_y, 20. / bezier_div_y, 60. / bezier_div_y, 60. / bezier_div_y};
    Bezier2D water(water_x, water_y, 4, .365);

    manager *myManager;
    myManager = new manager();

    // Scene One
//    myManager->addSphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(.999, .999, .999), SPEC, "./textures/art_normal.jpg",
//                         6000, 1);//Left
//    myManager->addSphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(.999, .999, .999), SPEC, "./textures/art_normal.jpg",
//                         6000, 1);//Right
//    myManager->addSphere(1e5, Vec(50, 40.8, 1e5), Vec(.75, .75, .75), DIFF, "./textures/marble.bmp", 4000);//Back
//    myManager->addSphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(.0,0,0), DIFF);//Front
//    myManager->addSphere(1e5, Vec(50, 1e5, 81.6), Vec(.8, .8, .8), DIFF, "./textures/star.jpg", 4000);//Bottom
//    myManager->addSphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(.75, .75, .75), DIFF);//Top
//    myManager->addBezier(Vec(30, 0, 40), Vec(.999, .999, .999), MIXED, bezier, "./textures/vase.bmp", 1);
//    myManager->addSphere(2, Vec(30, 34, 40), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(35.4, 38, 40), Vec(.4, .7, .9), SPEC);
//    myManager->addSphere(2, Vec(41.5, 2, 40), Vec(.9, .9, .3), REFR);
//    myManager->addMesh(Vec(70, 0, 40), Vec(1, .7, .4) * .999, REFR, 0.48, "./meshes/dinosaur.2k.obj", Vec(3,1,2));
//
//    myManager->addSphere(2, Vec(70, 2, 90), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(74, 2, 90), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(70, 2, 94), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(74, 2, 94), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(78, 2, 90), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(78, 2, 94), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(78, 2, 98), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(70, 2, 98), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(74, 2, 98), Vec(.9, .7, .7), REFR);
//    myManager->addSphere(2, Vec(72, 2 + sqrt(8), 92), Vec(.9, .9, .3), REFR);
//    myManager->addSphere(2, Vec(76, 2 + sqrt(8), 92), Vec(.9, .9, .3), REFR);
//    myManager->addSphere(2, Vec(72, 2 + sqrt(8), 96), Vec(.9, .9, .3), REFR);
//    myManager->addSphere(2, Vec(76, 2 + sqrt(8), 96), Vec(.9, .9, .3), REFR);
//    myManager->addSphere(2, Vec(74, 2 + 2*sqrt(8), 94), Vec(.4, .7, .9), REFR);
//
//    myManager->addMesh(Vec(50, 0, 110), Vec(.9, .1, .1) * .999, SPEC, 15, "./meshes/arma_0.01.obj", Vec(1,2,-3));
//
//    myManager->addSphere(2, Vec(21, 2, 90), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(25, 2, 90), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(21, 2, 94), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(25, 2, 94), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(29, 2, 90), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(29, 2, 94), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(29, 2, 98), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(21, 2, 98), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(25, 2, 98), Vec(.4,.7,.9), SPEC);
//    myManager->addSphere(2, Vec(23, 2 + sqrt(8), 92), Vec(.9,.9,.3), SPEC);
//    myManager->addSphere(2, Vec(27, 2 + sqrt(8), 92), Vec(.9,.9,.3), SPEC);
//    myManager->addSphere(2, Vec(23, 2 + sqrt(8), 96), Vec(.9,.9,.3), SPEC);
//    myManager->addSphere(2, Vec(27, 2 + sqrt(8), 96), Vec(.9,.9,.3), SPEC);
//    myManager->addSphere(2, Vec(25, 2 + 2*sqrt(8), 94), Vec(.9, .7, .7), SPEC);
//    myManager->addBezier(Vec(50, 40, 85), Vec(0.99, 0.99, 0.99), REFR, water);


    // Scene Two
    myManager->addSphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(.75, .25, .25), SPEC, "./textures/brickwall_normal.jpg",
                         6000, 1);//Left
    myManager->addSphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(.75, .25, .25), SPEC, "./textures/brickwall_normal.jpg",
                         6000, 1);//Right
    myManager->addSphere(1e5, Vec(50, 40.8, 1e5), Vec(.75, .75, .75), DIFF, "./textures/marble.bmp", 4000);//Back
    myManager->addSphere(1e5, Vec(50, 1e5, 81.6), Vec(.8, .8, .8), DIFF, "./textures/star.jpg", 4000);//Bottom
    myManager->addSphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(.75, .75, .75), DIFF);//Top
    myManager->addSphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(.0,0,0), DIFF);//Front
    myManager->addBezier(Vec(20, 0, 40), Vec(.999, .999, .999), MIXED, bezier, "./textures/vase.bmp", 1);
    myManager->addBezier(Vec(80, 0, 40), Vec(.999, .999, .999), MIXED, bezier, "./textures/vase.bmp", 1);
    myManager->addMesh(Vec(50, 0, 130), Vec(1, 1, 1) * .999, REFR, 22, "./meshes/arma_0.01.obj", Vec(1,2,-3));


    // Scene Three
//    myManager->addSphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(.999, .999, .999), DIFF);//Left
//    myManager->addSphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(.999, .999, .999), DIFF);//Right
//    myManager->addSphere(1e5, Vec(50, 40.8, 1e5), Vec(.999, .999, .999), DIFF);//Back
//    myManager->addSphere(1e5, Vec(50, 1e5, 81.6), Vec(.999, .999, .999), DIFF, "./textures/star.jpg", 4000);//Bottom
//    myManager->addSphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(.75, .75, .75), DIFF);//Top
//    myManager->addSphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(.0,0,0), DIFF);//Front
//
//    myManager->addMesh(Vec(50, 10, 85), Vec(0.999,0.999,0.999), REFR, 15, "./meshes/diamond.obj", Vec(1,3,2));

    // Scene Four
//    myManager->addSphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(.75, .25, .25), SPEC, "./textures/brickwall_normal.jpg",
//                         6000, 1);//Left
//    myManager->addSphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(.75, .25, .25), SPEC, "./textures/brickwall_normal.jpg",
//                         6000, 1);//Right
//    myManager->addSphere(1e5, Vec(50, 40.8, 1e5), Vec(.75, .75, .75), DIFF, "./textures/marble.bmp", 4000);//Back
//    myManager->addSphere(1e5, Vec(50, 1e5, 81.6), Vec(.8, .8, .8), DIFF, "./textures/star.jpg", 4000);//Bottom
//    myManager->addSphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(.75, .75, .75), DIFF);//Top
//    myManager->addSphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(.0,0,0), DIFF);//Front
//    myManager->addMesh(Vec(50, 0, 100), Vec(1, .9, .5) * .999, REFR, 30, "./meshes/bunny.obj", Vec(1,2,3));


    int w = ANTI_ALIASING ? 2 * width : width, h = ANTI_ALIASING ? 2 * height : height, samples = SAMPLENUM;
    Ray cam(Vec(50, 48, 295.6), Vec(0, -0.042612, -1).norm());

    double randx = enumerateRand() - 0.5;
    double randy = enumerateRand() - 0.5;
    double randz = (0.0 - cam.direction.x * randx - cam.direction.y * randx) / cam.direction.z;
    Vec random1 = Vec(randx, randy, randz).norm();
    Vec random2 = random1.cross(cam.direction).norm();

    Vec cx = Vec(w * .5135 / h), cy = (cx.cross(cam.direction)).norm() * .5135, *c = new Vec[w * h], vw;
    for (int y = 0; y < h; y++) {
        fprintf(stderr, "\rHitPointPass %5.2f%%", 100.0 * y / (h - 1));
        for (int x = 0; x < w; x++) {
            myManager->pixel_index = x + y * w;

            // Tent filter for soft shadow and anti-aliasing
            double r1 = 2 * enumerateRand(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
            double r2 = 2 * enumerateRand(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
            Vec d = cx * ((x + 0.5 + dx) / w - 0.5) + cy * (-(y + 0.5 + dy) / h + 0.5) + cam.direction;
            if (focalLength == 0) {
                myManager->measure(Ray(cam.origin + d * 140, d.norm()), 0, Vec(), Vec(1, 1, 1), 0);
            } else {
                // Set focal length and aperture for depth of focus
                d = d.norm() * focalLength;
                double rand1 = enumerateRand() * 2 - 1.0;
                double rand2 = enumerateRand() * 2 - 1.0;
                Vec v1 = random1 * rand1 * aperture;
                Vec v2 = random2 * rand2 * aperture;
                Vec sp = cam.origin + v1 + v2;
                Vec fp = d + cam.origin;
                myManager->measure(Ray(sp + (fp - sp).norm() * 140, (fp - sp).norm()), 0, Vec(), Vec(1, 1, 1),
                                   0);
            }
        }
    }
    fprintf(stderr, "\n");

    myManager->buildHashGrid(w, h);
    myManager->num_photon = samples;
    vw = Vec(1, 1, 1);
#pragma omp parallel for schedule(dynamic, 12)
    for (int i = 0; i < myManager->num_photon; i++) {
        double p = 100. * (i + 1) / myManager->num_photon;
        fprintf(stderr, "\rPhotonPass %5.2f%%", p);
        int m = 1000 * i;
        Ray r;
        Vec f;
        for (int j = 0; j < 1000; j++) {
            manager::generatePhoton(&r, &f, m + j);
            myManager->trace(r, 0, f, vw, m + j);
        }
    }

    List *lst = myManager->hashPoints;
    while (lst != nullptr) {
        HPoint *hp = lst->id;
        lst = lst->next;
        int i = hp->pix;
        c[i] = c[i] + hp->flux * (1.0 / (M_PI * hp->r2 * myManager->num_photon * 1000.0));
    }

    FILE *f = fopen("image.ppm", "w");
    if (!ANTI_ALIASING) {
        fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
        for (int i = 0; i < w * h; i++) {
            fprintf(f, "%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
        }
    } else {
        // Combine 4 pixels into 1 pixel for anti-aliasing
        fprintf(f, "P3\n%d %d\n%d\n", w / 2, h / 2, 255);
        for (int i = 0; i < w * h / 4; i++) {
            Vec tmpColor = Vec();
            int y = i % int((w / 2));
            int x = (i - y) / w * 2 + 1;
            int index = (2 * x - 2) * w + 2 * y - 1;
            tmpColor = tmpColor + Vec(clamp(c[index].x), clamp(c[index].y), clamp(c[index].z)) * .25;
            index += 1;
            tmpColor = tmpColor + Vec(clamp(c[index].x), clamp(c[index].y), clamp(c[index].z)) * .25;
            index += w;
            tmpColor = tmpColor + Vec(clamp(c[index].x), clamp(c[index].y), clamp(c[index].z)) * .25;
            index += 1;
            tmpColor = tmpColor + Vec(clamp(c[index].x), clamp(c[index].y), clamp(c[index].z)) * .25;
            fprintf(f, "%d %d %d ", toInt(tmpColor.x), toInt(tmpColor.y), toInt(tmpColor.z));
        }
    }
}