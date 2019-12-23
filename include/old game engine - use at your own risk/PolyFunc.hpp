c2Poly createPoly(int shape)
{
    c2Poly poly;
    switch (shape)
    {

    case TRIANGLE:
        // This defines a triangle in CCW order.
        poly.verts[0] = c2V(0.0f, 0.0f);
        poly.verts[1] = c2V(1.0f, 0.0f);
        poly.verts[2] = c2V(0.0f, 1.0f);

        c2MakePoly(&poly);
        break;

    case SQUARE:
        poly.verts[0] = c2V(0.0f, 0.0f);
        poly.verts[1] = c2V(1.0f, 0.0f);
        poly.verts[2] = c2V(0.0f, 1.0f);
        poly.verts[3] = c2V(1.0f, 1.0f);

        c2MakePoly(&poly);
        break;
    default:
        break;
    }
    return poly;
}