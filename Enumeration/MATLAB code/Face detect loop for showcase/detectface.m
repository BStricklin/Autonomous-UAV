function detectface(img)
faceDetector=vision.CascadeObjectDetector;
bboxes=step(faceDetector,img);
Ifaces=insertObjectAnnotation(img,'rectangle',bboxes,'Face');
imagesc(Ifaces);
title('detect faces');
drawnow;
end