# Explanation of Scene

This idea actually came up from one of my math professors here at Cal Poly.

The story is of a people called the "vector people". They are but an ordinary group of fellows like you and I, only except they merely live in the second dimension. They can live, breathe, eat, and talk like us, but they can never fully comprehend the 3rd dimension like us humans. Only with the power of mathematics, and the power of the lifting matrix, can they begin to comprehend the beatuy of higher dimensions. 

I chose this scene because it really does express my love for mathematics. There's always a hidden beauty behind it that people seem to misconstrue, likely because they find math boring or very tedious in terms of calculation. The more simpler ideas that come from mathematics tend to be the ones that are the most beautiful. Like with the vector people, there's more than just meets what we see and observe. In our eyes, there could be some unseeable 4th dimension that we just can't comprehend. But that doesn't stop us from using mathematics to try to understand it better. 

# Complexity of the Scene

While this scene is pretty bare bones in terms of presentation, I actually had to do a lot in making many of the materials:
- The "dummy.obj" was my multimesh object, which I had to support drawing
- The "graph.obj" that gets generated on a run is a custom obj designed by yours truly. It *actually* generates the two dimensional surface $f(x,y)$ given in it's construction call. The one demoed for you is the function $f(x,y) = 10e^{-|x| - |y|}\sin(x)\sin(y)$, which is constructed by sampling points in such a surface and creating vertices/faces from it. 
- The "plane.obj" is also constructed by just using a simple $f(x,y) = 0$ surface.

I could've actually constructed some other objects, for instance a "vector.obj" was in the works where it's composed of a cylinder (the surface $z^2 + x^2 = 0$) and a right-angle cone ($z = x^2 + y^2$) on one end of the cylinder; however, this was scrapped mid development. 

Another thing is that I also wanted to try automating the matrix stack for related animations. Hence, the `MeshContainer.h` and `Object.h` files automate both the loading of objs as well as the placing and transforming of components. Things included in these files include:
- Having multiples of the same object within the same scene, with choice of importing or removing imported transformations
- Importing transformations from one object to another.
- Copying an objects transformations
- ... 

In short, the idea of the matrix stack is actually contained in the `Object.h` file, and I had to make sure that that worked on a small group of objects before building my scene. 

# Reflection

Looking for .objs, I noticed that there definitely wasn't a set standard. I needed some cloud.objs and looking for specifically low poly ones to save on memory was a bit hard. Sometimes certain objs aren't in the right format, or aren't quite exactly what you are looking for in terms of design. 