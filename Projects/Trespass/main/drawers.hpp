void DrawRectWithOpenGL(glRecti *pRect)
{
  glPushMatrix();                         //Make sure our transformations don't affect any other transformations in other code
  glTranslatef(pRect->x, pRect->y, 0.0f); //Translate rectangle to its assigned x and y position
  //Put other transformations here
  glBegin(GL_QUADS);  //We want to draw a quad, i.e. shape with four sides
  glColor3f(1, 0, 0); //Set the colour to red
  glVertex2f(0, 0);   //Draw the four corners of the rectangle
  glVertex2f(0, pRect->h);
  glVertex2f(pRect->w, pRect->h);
  glVertex2f(pRect->w, 0);
  glEnd();
  glPopMatrix();
}
