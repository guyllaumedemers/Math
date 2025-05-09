# Math

#### Project

A `Sandbox` project built for visualizing mathematical functions using Opengl rendering library. In this project, I lay core knowledge about mathematics and it's usage in the context of game development. As you may suspect, most of the content defined in code is accompany by comments/thoughts, as **I am learning these concepts myself**. This project is **purely educational** and has **no time limit!** Books, Pen and paper are my best friend here.

#### What this README.md is not

What you will find in this project may not always be the most optimized approach and it's alright! More often than not, I will make my how implementation of existing api handled by third-party library just to prove myself that I understand the concept I am interacting with. I haven't found a better way of learning something new and strongly believe that this is the right approach!

#### Resources

* [uofm](https://youtube.com/@uofmintroductiontocomputer5167?si=5C0YrTTMzXKvltTH)
* [scratchapixel](https://www.scratchapixel.com/index.html)
* [gingerBill](https://www.gingerbill.org/)
* [game engine development book](https://a.co/d/irCVusP)
* [real time rendering book](https://a.co/d/ffQ7mah)
* [pbr book](https://a.co/d/8LuM4Oq)

## Coordinate system

A set of coordinate systems to be used in order to create a 3d World and render it's content on a 2d screen.

* Object (Local)
* World
* View (Camera)
* Screen (NDC or Clip)
* Raster

#### Object Space (Local Space)

Define an OrthoNormal set of axis rooting the **Model** pivot point to it's Origin. Usually defined by 3d Modeling software in which the **Model** was created in.

#### World Space

Define an imaginary position, usually the zero vector, from which every other **Objects** are rooted to. When defining the transforms of an **Object**, we more often than not, refer to it in regards to the World and like in **Object Space**, the World define an OrthoNormal set of axis where the pivot point and it's Origin are identical.

#### View Space (Camera Space)

Define the coordinate space of an Object, i.e the **Camera Object**. Like any other Object, it's rooted to the World and define an OrthoNormal set of axis where the pivot point and it's Origin are identical. Camera space is particularly important when rendering our 3d world onto a 2d surface. Using basic trigonometry, we can project 3d points onto a 2d surface and visuallize a representation of a **virtual** world. (We will further extend as we go - see **FCamera.cc** for implementation details. Or do like me and take a pen and paper to do the calculation yourself!)

#### Screen Space (Normalize Device Coordinate or Clip Space)

Define a coordinate space on a 2d surface that remaps points inside an **imaginary** view volume, used to define limits and prevent rendering to infinity, into Normalized Device Coordinate, i.e [-1, 1]. Doing so allow us to : A) Discard any information that is outside these **user-provided** limits and B) Remaps normalized position to our Viewport in the Raster space stage.

Note : As you may have notice, people use different names for this coordinate system. Personnaly, I would rather use **Image Space** or **NDC**. It's more explicit and inline with the operation executed. Explanation : Our 3d points are projected onto the 2d **Image plane** and remapped to the range [-1, 1] (into **Normalized Device Coordinate**).

#### Raster Space (Pixel Space)

Define a coordinate space on a 2d surface that match the aspect ratio of your **Viewport** screen. A 3d point, projected onto a 2d surface will occupy a region of pixels from which color information will be outputed. Depth information are taken into account for each pixel and give priority to the projected point that is closer to the Image plane.

## How to convert from 3d to 2d

In the following section, I'll be going over `Perspective` projection and `View frustum`. I'll provide resources I personnaly used to learn basics on math allowing us to convert from one coordinate system to the other and how a 3d object can be converted onto a 2d plane (screen).

#### Perspective

Perspective projection involve the conversion of a point in 3d space onto a 2d plane called `Projection` plane. Using basic trigonometry, and angle sum identities, we can solve for the screen space position of a projected point using the intersect created between the camera origin and the 3d space point.

##### Math

Defining our View frustum (and projection plane)

```
What are we trying to prove ?

* Field of View (angle).
* Distance between Camera origin and projection plane (g).
* Near, Far distance of the View frustum (n)(f).

How to project a point from 3d space onto a 2d plane ?

legend :

* Ps(x, y, z) - Point in Screen
* Pw(x, y, z) - Point in World

If we look at the View frustum from the side, we see that we are forming a right triangle between the camera z-axis and the point in 3d space. Our projection plane distance from the camera and fov will determine the trace intersection with the plane.

* Using basic trig, we can find the components of our right triangles with : tan(angle) = opposite / adjacent.
* And knowing that both triangles have congruent angles, we can say that : Pw(y) / Pw(z) equals Ps(y) / g.

Projection & Perspective Divide - solving for Ps(y)

* Ps(y) = (Pw(y) * g) / Pw(z)

Notes : Pw(z) perform perspective division to account for object being farther way appearaing smaller on the screen. 
Notes : Projected point should be between the following bounds of our projection plane to be displayed within the bounds of our screen.

l <= Ps(x) <= r
b <= Ps(y) <= t
f <= Ps(z) <= n

in order to simplify our approach, and math, working with normalized values will be required:

legend :

* l=left, r=right
* b=bottom, t=top
* n=near, f=far

Perspective Scaling - solving for Ps(y), [-1,1]

* b - b <= Ps(y) - b <= t - b equals 0 <= Ps(y) - b <= t - b

* 0 / (t - b) <= (Ps(y) / (t - b)) - (b / (t - b)) <= (t - b) / (t - b) equals 0 <= (Ps(y) / (t - b)) - (b / (t - b)) <= 1

* 0 * 2 <= (2Ps(y) / (t - b)) - (2b / (t - b)) <= 1 * 2 equals 0 <= (2Ps(y) / (t - b)) - (2b / (t - b)) <= 2

* 0 - 1 <= (2Ps(y) / (t - b)) - (2b / (t - b)) - 1 <= 2 - 1 equals -1 <= (2Ps(y) / (t - b)) - (2b / (t - b)) - ((t - b) / (t - b)) <= 1

* (2Ps(y) / (t - b)) - (2b / (t - b)) - ((t - b) / (t - b)) is equivalent to  (2Ps(y) / (t - b)) + (-2b / (t - b)) + ((-t + b) / (t - b)) or (2Ps(y) / (t - b)) + (-t - b / (t - b))

finally...

* -1 <= (2Ps(y) / (t - b)) - ((t + b) / (t - b)) <= 1

Now, through substitution of Ps(y), we can say that to project a point 'P' in world, onto a 2d plane, we have to :

* (2Pw(y) / (Pw(z) * (t - b))) - ((t + b) / (t - b))

However, we do not currently know 'g'.

to find our distance between the camera from the projection plane, we have to go back to the initial View frustum and look at how the Field of view impact distance.

* we see from the side that our field of view was seperate in two and form the original right triangle used to determine the Ps(y).
* we also remember that in the previous step, we normalized our projection plane to be between a height of [-1,1] so we can later scale our points based on the screen viewport.

this mean that the above statement regarding basic trigs : tan(angle) = opposite / angle, has now become : tan(angle/2) = opposite / angle. 

solving for g

* g = 1 / tan(angle/2)

and from there, we can determine the distance based on fov. But also, complete the previous equation for Ps(y).

* (2Pw(y) / (tan(fov/2) * Pw(z) * (t - b))) - ((t + b) / (t - b))

```

#### Cannonical View Model

A box where bounds remain parallel to their original direction and have no convergion point due to parallelism.

#### View Frustum

A pyramid, with it's top cut off, where bounds converge toward a singular point of interest.

![view-frustum](https://github.com/guyllaumedemers/Math/blob/master/Res/ViewFrustum.png)

#### Post-projection Task handled by Graphic Pipeline

tbd

* Clipping
* Perspective divide
* Viewport transform
* Rasterization

![clipping](https://github.com/guyllaumedemers/Math/blob/master/Res/VertexTransformPipeline.png)

#### Clipping

tbd

#### Perspective divide

tbd

#### Viewport transform

tbd

#### Rasterization

tbd
