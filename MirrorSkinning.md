# Mirror Skinning #
Have both sides (left/right, or top/bottom) of a model use the same half-of-object texture.
Split into left and right and use the same half-of-object image, so its easier to modify both sides and have it be uniform.
Either use the same image for both sides or copy-edit one side from the other.

## How to in Wings3D ##
### Cloning the UV ###
  * 1. Remove half of the object
  * 2. Merge any reminding faces to only one face
  * 3. Select the face where the objects will connect. In the object explorer? right click "_hole_" click apply to selected.
  * 4. Select the object, deselect the face where the objects will connect. UV map the selected faces of the object
  * 5. Select the face where the objects will connect.
  * 6. Create a virtual mirror (Tools -> Virtual Mirror -> Create)

Both sides now use the same texture and UV mapping. You may wish to split into 2 object.

### Splitting the model ###
  * 7. "Freeze" the mirror (Tools -> Virtual Mirror -> Freeze) so it is no longer virtual
  * 8. In line mode select all of the lines where the halfs connect
  * 9. Right click -> Loopcut

Now the two objects uses the same texture.