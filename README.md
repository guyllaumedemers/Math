# Math

#### Project

A `Sandbox` project built for visualizing mathematical functions using Opengl rendering library.

#### What this README.md is not

tbd

## Coordinate system

tbd

* Object (Local)
* World
* View (Camera)
* Screen (NDC or Clip)
* Raster

#### Object Space (Local Space)

tbd

#### World Space

tbd

#### View Space (Camera Space)

tbd

#### Screen Space (Normalize Device Coordinate or Clip Space)

tbd

#### Raster Space (Pixel Space)

tbd

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

#### View Frustum

tbd

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