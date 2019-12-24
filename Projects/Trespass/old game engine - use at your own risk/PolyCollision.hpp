
struct Polygon
{
    std::vector<glm::vec2> vertices;
    int x, y;
};


glm::vec2 transform_verts(glm::vec2 verts, glm::vec2 transformation)
{
    verts.x = verts.x * transformation.x;
    verts.y = verts.y * transformation.y;
    return verts;
}

std::vector<glm::vec2> transform_vertices(std::vector<glm::vec2> vertices, glm::vec2 transformation)
{   
    int count = vertices.size();


    
    for(size_t i = 0; i < count; i++)
    {
        vertices[i] = transform_verts(vertices[i], transformation);
    }
    return vertices;
}

bool PolygonCollision(Polygon object, Polygon object2)
{
    int intersects = 0;
    int count = object.vertices.size();
    for(size_t i = 0; i < count; i++)
    {
        int count2 = object2.vertices.size();
        for(size_t i = 0; i < count2; i++)
        {
            if (((y1 <= y && y < y2) || (y2 <= y && y < y1)) && x < ((x2 - x1) / (y2 - y1) * (y - y1) + x1))
        }
        
    }
    
}