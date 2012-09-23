#include <stdio.h>
#include <QtCore>

void print_branch_end(const char* texture, const char* scale)
{
    printf("    pigment { image_map {\"%s\"} scale %s }\n", texture, scale);
    printf("    finish {ambient 0.8 diffuse 0.8 phong 0.2}\n");
    printf("}\n\n");
}

int main(int argc, char* argv[])
{
    if (argc != 5) {
        printf("Usage: brc2pov.exe filename.brc cubic[linear] texture.jpg scale");
        exit(-1);
    }

    QFile brcfile(argv[1]);
    if (!brcfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("Can not open brc file %s", argv[1]);
        exit(-2);
    }
    QTextStream in(&brcfile);
    bool file_begin = true;
    while (!in.atEnd()) {
        QStringList items = in.readLine().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (items.empty())
            continue;
        if (items[0] == "g") {
            if (!file_begin) {
                print_branch_end(argv[3], argv[4]);
            }
            if (file_begin) {
                file_begin = false;
            }
            printf("sphere_sweep {\n");
            printf("    %s_spline\n", argv[2]);
        }
        if (items[0] == "p") {
            printf("    %d,\n", items[4].toInt());
        }
        if (items[0] == "v") {
            printf("    <%f, %f, %f>, %f\n", items[1].toFloat(), items[2].toFloat(), items[3].toFloat(), items[4].toFloat());
        }
    }
    print_branch_end(argv[3], argv[4]);
    return 0;
}
