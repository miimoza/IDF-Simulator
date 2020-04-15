echo "Generating map.png with dot..."
dot -Tpng output.dot -o map.png
#neato -Goverlap=false -Gmodel=subset -Tpng output.dot -o map.png
