/**************************************************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
#include "kgl_lib_pch.h"
#include "kgl_aabb.h"
#include "kgl_plane.h"
#include "kgl_view_frustum.h"

namespace kgl
{
#define ANG2RAD 3.14159265358979323846/180.0
    ViewFrustum::ViewFrustum() 
    {
    }

    ViewFrustum::~ViewFrustum() 
    {
    }

    void ViewFrustum::setCamInternals(float angle, float ratio, float nearD, float farD) 
    {
        this->ratio = ratio;
        this->angle = angle;
        this->nearD = nearD;
        this->farD = farD;

        tang = (float)tan(angle* ANG2RAD * 0.5);
        nh = nearD * tang;
        nw = nh * ratio;
        fh = farD  * tang;
        fw = fh * ratio;
    }


    void ViewFrustum::setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u)
    {
        glm::vec3 nc;
        glm::vec3 fc;
        glm::vec3 X;
        glm::vec3 Y;
        glm::vec3 Z;

        Z = glm::normalize(p - l);
        X = glm::normalize(u * Z);

        Y = Z * X;

        nc = p - Z * nearD;
        fc = p - Z * farD;

        ntl = nc + Y * nh - X * nw;
        ntr = nc + Y * nh + X * nw;
        nbl = nc - Y * nh - X * nw;
        nbr = nc - Y * nh + X * nw;

        ftl = fc + Y * fh - X * fw;
        ftr = fc + Y * fh + X * fw;
        fbl = fc - Y * fh - X * fw;
        fbr = fc - Y * fh + X * fw;

        pl[TOP].SetThreePoints(ntr, ntl, ftl);
        pl[BOTTOM].SetThreePoints(nbl, nbr, fbr);
        pl[LEFT].SetThreePoints(ntl, nbl, fbl);
        pl[RIGHT].SetThreePoints(nbr, ntr, fbr);
        pl[NEARP].SetThreePoints(ntl, ntr, nbr);
        pl[FARP].SetThreePoints(ftr, ftl, fbl);
    }


    ViewFrustum::Intersection ViewFrustum::PointInFrustum(const glm::vec3 &p)
    {
        Intersection result = Intersection::INSIDE;
        
        for (int i = 0; i < 6; i++)
        {
            if (pl[i].Distance(p) < 0)
                return Intersection::OUTSIDE;
        }

        return result;
    }

    ViewFrustum::Intersection ViewFrustum::SphereInFrustum(const glm::vec3 &p, float raio) {

        Intersection result = Intersection::INSIDE;
        float distance;

        for (int i = 0; i < 6; i++)
        {
            distance = pl[i].Distance(p);
            
            if (distance < -raio)
                return Intersection::OUTSIDE;
            else if (distance < raio)
                result = Intersection::INTERSECT;
        }

        return(result);
    }


    ViewFrustum::Intersection ViewFrustum::AABoxInFrustum(const AABB &b)
    {
        Intersection result = Intersection::INSIDE;
        glm::vec3 v(0);
        
        for (int i = 0; i < 6; i++)
        {
            const glm::vec3& n = pl[i].Normal();
            v = b.getVertexP(n);

            if (pl[i].Distance(v) < 0)
                return OUTSIDE;
            else
            {
                v = b.getVertexN(n);
                if (pl[i].Distance(v) < 0)
                    result = INTERSECT;
            }
        }
        
        return result;
    }

    void ViewFrustum::drawPoints() 
    {
        /*
        glBegin(GL_POINTS);

        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(nbl.x, nbl.y, nbl.z);
        glVertex3f(nbr.x, nbr.y, nbr.z);

        glVertex3f(ftl.x, ftl.y, ftl.z);
        glVertex3f(ftr.x, ftr.y, ftr.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);

        glEnd();
        */
    }


    void ViewFrustum::drawLines() 
    {
        /*
        glBegin(GL_LINE_LOOP);
        //near plane
        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(nbr.x, nbr.y, nbr.z);
        glVertex3f(nbl.x, nbl.y, nbl.z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        //far plane
        glVertex3f(ftr.x, ftr.y, ftr.z);
        glVertex3f(ftl.x, ftl.y, ftl.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        //bottom plane
        glVertex3f(nbl.x, nbl.y, nbl.z);
        glVertex3f(nbr.x, nbr.y, nbr.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        //top plane
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(ftl.x, ftl.y, ftl.z);
        glVertex3f(ftr.x, ftr.y, ftr.z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        //left plane
        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(nbl.x, nbl.y, nbl.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);
        glVertex3f(ftl.x, ftl.y, ftl.z);
        glEnd();

        glBegin(GL_LINE_LOOP);
        // right plane
        glVertex3f(nbr.x, nbr.y, nbr.z);
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(ftr.x, ftr.y, ftr.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);

        glEnd();
        */
    }


    void ViewFrustum::drawPlanes()
    {
        /*
        glBegin(GL_QUADS);

        //near plane
        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(nbr.x, nbr.y, nbr.z);
        glVertex3f(nbl.x, nbl.y, nbl.z);

        //far plane
        glVertex3f(ftr.x, ftr.y, ftr.z);
        glVertex3f(ftl.x, ftl.y, ftl.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);

        //bottom plane
        glVertex3f(nbl.x, nbl.y, nbl.z);
        glVertex3f(nbr.x, nbr.y, nbr.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);

        //top plane
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(ftl.x, ftl.y, ftl.z);
        glVertex3f(ftr.x, ftr.y, ftr.z);

        //left plane

        glVertex3f(ntl.x, ntl.y, ntl.z);
        glVertex3f(nbl.x, nbl.y, nbl.z);
        glVertex3f(fbl.x, fbl.y, fbl.z);
        glVertex3f(ftl.x, ftl.y, ftl.z);

        // right plane
        glVertex3f(nbr.x, nbr.y, nbr.z);
        glVertex3f(ntr.x, ntr.y, ntr.z);
        glVertex3f(ftr.x, ftr.y, ftr.z);
        glVertex3f(fbr.x, fbr.y, fbr.z);

        glEnd();
        */

    }

    void ViewFrustum::drawNormals()
    {
        /*
        glm::vec3 a, b;

        glBegin(GL_LINES);

        // near
        a = (ntr + ntl + nbr + nbl) * 0.25;
        b = a + pl[NEARP].normal;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

        // far
        a = (ftr + ftl + fbr + fbl) * 0.25;
        b = a + pl[FARP].normal;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

        // left
        a = (ftl + fbl + nbl + ntl) * 0.25;
        b = a + pl[LEFT].normal;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

        // right
        a = (ftr + nbr + fbr + ntr) * 0.25;
        b = a + pl[RIGHT].normal;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

        // top
        a = (ftr + ftl + ntr + ntl) * 0.25;
        b = a + pl[TOP].normal;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

        // bottom
        a = (fbr + fbl + nbr + nbl) * 0.25;
        b = a + pl[BOTTOM].normal;
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);

        glEnd();

        */
    }
}