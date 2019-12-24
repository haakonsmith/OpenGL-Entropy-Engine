

class ShapeFactory
{
public:


  static std::unique_ptr<ShapeBase> createShape(GLuint shader_program_id, GLuint init_memory_type, int shape)
  {
    switch (shape)
    {
    case TRIANGLE:
      return std::make_unique<Triangle>(shader_program_id, init_memory_type); 
      break;

      case OBJ:
        return std::make_unique<Obj>(shader_program_id, init_memory_type);
        break;
    }
    return nullptr;
  }
};