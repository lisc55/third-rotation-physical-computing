set -e

assignment=$1
assignment_name=${assignment:3}

cmake --build ./build --config Release --target $assignment_name


build/proj/$assignment/$assignment_name -o build/proj/$assignment_name/output

./viewer/bin/linux/opengl_viewer -o build/proj/$assignment_name/output

ffmpeg -start_number 0 -framerate 50 -i build/proj/$assignment_name/output/_images/%04d.png  -vcodec libx264 -crf 25 -pix_fmt yuv420p -y $assignment_name.mp4

