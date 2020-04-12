TEST_CASE("Texture load", "[Entropy]")
{
    const std::string file;
    Texture tex(file);
    CHECK(tex.path == file);

    auto glError = glGetError();

    INFO("OpengGL error code: " << glError)
    CHECK(glError);
}