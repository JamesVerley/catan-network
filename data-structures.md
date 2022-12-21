# The board
### Map
hexes are stored in a 2d array with a width and height.
Given a tileId, neighbors are found as follow

id_bottomleft(id) id+boardWidth-1 + (id/boardWidth+1)%2
id_bottomright(id) id+boardWidth + (id/boardWidth+1)%2
id_right(id) id+1
id_left(id) id-1
id_topleft(id) id-boardWidth-1 + (id/boardWidth-1)%2
id_topright(id) id-boardWidth + (id/boardWidth-1)%2

And less usefully since x and y are confusing:

bottomleft(x,y) x-1+y%2 + (y+1)*boardWidth
bottomright(x,y) x+y%2 + (y+1)*boardWidth
right(x,y) x+1 + y*boardWidth
left(x,y) x-1 + y*boardWidth
topleft(x,y) x-1+y%2 + (y-1)*boardWidth
topright(x,y) x+y%2 + (y-1)*boardWidth

### Roads and buildings
Each hex points to 3 road placements and 3 building placements,
building places starting from the leftmost vertex going clockwise
edge starting from the topleft going clockwise

###  Harbours / Ports
