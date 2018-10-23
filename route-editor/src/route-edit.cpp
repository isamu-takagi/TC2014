#include "route-edit.hpp"
#include "tkglib/tkg-utility.hpp"
#include <fstream>
#include <sstream>
#include <QTableWidget>
#include <QColor>
#include <QMessageBox>

#define POS(waypoint) (tkg::Point3(waypoint.x, waypoint.y, 0))

std::vector<tkg::Point3> RouteEdit::inner_box(int i)
{
    tkg::Point3 v = (POS(node[i+1]) - POS(node[i])).unit();
    tkg::Point3 w = v.rotateZ(tkg::pi/2);

    std::vector<tkg::Point3> ret(5);
    ret[0] = POS(node[i  ]) + w * node[i].left_length  - v * node[i].rear_length;
    ret[1] = POS(node[i+1]) + w * node[i].left_length  + v * node[i].front_length;
    ret[2] = POS(node[i+1]) - w * node[i].right_length + v * node[i].front_length;
    ret[3] = POS(node[i  ]) - w * node[i].right_length - v * node[i].rear_length;
    ret[4] = ret[0];
    return ret;
}

std::vector<tkg::Point3> RouteEdit::outer_box(int i)
{
    tkg::Point3 v = (POS(node[i+1]) - POS(node[i])).unit();
    tkg::Point3 w = v.rotateZ(tkg::pi/2);

    std::vector<tkg::Point3> ret(5);
    ret[0] = POS(node[i  ]) + w * (node[i].left_length  + detect) - v * node[i].rear_length;;
    ret[1] = POS(node[i+1]) + w * (node[i].left_length  + detect) + v * node[i].front_length;
    ret[2] = POS(node[i+1]) - w * (node[i].right_length + detect) + v * node[i].front_length;
    ret[3] = POS(node[i  ]) - w * (node[i].right_length + detect) - v * node[i].rear_length;;
    ret[4] = ret[0];
    return ret;
}

RouteEdit::RouteEdit(tkg::ConfigFile &conf)
{
    disp_area   = true;
    update      = false;
    node_select = -1;
    edge_select = -1;
    tmplog.type = NONE;

    file        = conf["Route"]["file"];
    detect      = tkg::parseInt(conf["Route"]["detect-length"]) / 1000.0;
    avoid       = tkg::parseInt(conf["Route"]["avoid-length"])  / 1000.0;
    text_size   = tkg::parseInt(conf["Route"]["text-size"])     / 1000.0;

    text_color  = tkg::Color4(conf["Route"]["text-color"]);
    inner_color = tkg::Color4(conf["Route"]["innerbox-color"]);
    outer_color = tkg::Color4(conf["Route"]["outerbox-color"]);

    node_color[0] = tkg::Color4(conf["Route"]["normal-node-color"]);
    node_color[1] = tkg::Color4(conf["Route"]["seamless-node-color"]);
    edge_color[0] = tkg::Color4(conf["Route"]["normal-edge-color"]);
    edge_color[1] = tkg::Color4(conf["Route"]["search-edge-color"]);

    /*
    for(char i=0; i<26; i++)
    {
        node_color[i] = tkg::Color4(conf["Route"][std::string("node-color-") + (char)(i+'A')]);
        edge_color[i] =  + (char)(i+'A')]);
    }
    */

    for(int i=1; true; i++)
    {
        std::string group = tkg::strf("ReferenceRoute%d", i);

        if(conf[group].empty()) break;
        reference_file .push_back( conf[group]["file"] );
        reference_color.push_back( tkg::Color4(conf[group]["color"]) );
    }
}

void RouteEdit::toggleArea()
{
    disp_area = !disp_area;
}

void RouteEdit::load()
{
    readWayPointFile( file, node );
    update = true;
}

void RouteEdit::loadReference()
{
    for(uint i=0; i<reference_file.size(); i++)
    {
        std::ifstream fin(reference_file[i].c_str());
        if( !fin ) return;

        tkg::Point3 p;
        reference_node.push_back( std::vector<tkg::Point3>() );

        while(fin >> p.x >> p.y)
        {
            reference_node.back().push_back( p );
        }
    }
}

void RouteEdit::save()
{
    // チェック
    for(uint i=0; i<node.size(); i++)
    {
        if(node[i].velocity < 0.3)
        {
            QMessageBox msgBox(NULL);
            msgBox.setText("設定速度が0.3未満のウェイポイントが存在します");
            msgBox.exec();
            break;
        }
    }

    writeWayPointFile(file, node);
}

void RouteEdit::WriteTableText(QString *text, const WayPointData &waypoint)
{
    char str[256];
    text[0] = tkg::cstrf(str, "%+.1f", waypoint.x);
    text[1] = tkg::cstrf(str, "%+.1f", waypoint.y);
    text[2] = tkg::cstrf(str, "%+.1f", waypoint.velocity);
    text[3] = tkg::cstrf(str, "%d",    waypoint.seamless);
    text[4] = tkg::cstrf(str, "%+.2f", waypoint.seamless_length);
    text[5] = tkg::cstrf(str, "%+.2f", waypoint.right_length);
    text[6] = tkg::cstrf(str, "%+.2f", waypoint.left_length );
    text[7] = tkg::cstrf(str, "%+.2f", waypoint.front_length);
    text[8] = tkg::cstrf(str, "%+.2f", waypoint.rear_length );
    text[9] = tkg::cstrf(str, "%d",    waypoint.search.enable_searching);
}

void RouteEdit::drawTable(QTableWidget *table)
{
    if(!update) return;

    QString text[11];

    table->setRowCount(0);
    for(uint i=0; i<node.size(); i++)
    {
        text[0] = tkg::strf("%d", i).c_str();
        WriteTableText( text + 1, node[i] );

        table->insertRow(i);
        for(int j = 0; j < 11; ++j)
        {
            table->setItem(i, j, new QTableWidgetItem(text[j]));
        }

        QColor color_vel = QColor(255,0,0);
        if(node[i].velocity > 0.1) color_vel = QColor(255,255,150);
        if(node[i].velocity > 0.5) color_vel = QColor(200,255,200);
        table->item(i,3)->setBackgroundColor( color_vel );

        QColor color_flag = QColor(255,200,255);
        if(node[i].seamless               ) table->item(i, 4)->setBackgroundColor( color_flag );
        if(node[i].search.enable_searching) table->item(i,10)->setBackgroundColor( color_flag );
    }

    update=false;
}

void RouteEdit::readTable(QTableWidget *table)
{
    char str[256];
    WayPointData waypoint;
    QString old_text[10];
    QString new_text[10];

    for(int i = 0; i < table->rowCount(); i++)
    {
        // Noは変更不可

        tkg::cstrf(str, "%d", i);
        if( table->item(i,0)->text() != str ) table->item(i,0)->setText( str );

        // 新しい数値を読み込み変更されていたらログをとる
        waypoint = node[i];
        WriteTableText(old_text, node[i]);
        node[i].x                       = table->item(i, 1)->text().toDouble();
        node[i].y                       = table->item(i, 2)->text().toDouble();
        node[i].velocity                = table->item(i, 3)->text().toDouble();
        node[i].seamless                = table->item(i, 4)->text().toInt();
        node[i].seamless_length         = table->item(i ,5)->text().toDouble();
        node[i].right_length            = table->item(i ,6)->text().toDouble();
        node[i].left_length             = table->item(i ,7)->text().toDouble();
        node[i].front_length            = table->item(i ,8)->text().toDouble();
        node[i].rear_length             = table->item(i ,9)->text().toDouble();
        node[i].search.enable_searching = table->item(i,10)->text().toInt();
        WriteTableText(new_text, node[i]);

        bool changed = false;
        for(int j=0; j<10; j++)
        {
            if(old_text[j] != new_text[j]) changed = true;

            /*table->setItem(i, j, new QTableWidgetItem(QString(new_str[j])));*/
        }

        if(changed)
        {
            update = true;

            tmplog.type   = MOVE;
            tmplog.data   = waypoint;
            tmplog.select = i;
            log.push_back(tmplog);
            tmplog.type   = NONE;
        }
    }
}

void RouteEdit::draw(double scale)
{
    glColor3d(1.0, 1.0, 0.0);

   // Box ------------------------------------------
    if( disp_area )
    {
        for(uint i=0; i+1<node.size(); i++)
        {
            std::vector<tkg::Point3> outer = outer_box(i);
            glColor4dv(outer_color.rgba);
            glBegin(GL_LINE_STRIP);
            for(int j=0; j<5; j++) tkg::glVertex(outer[j]);
            glEnd();

            std::vector<tkg::Point3> inner = inner_box(i);
            glColor4dv(inner_color.rgba);
            glBegin(GL_LINE_STRIP);
            for(int j=0; j<5; j++) tkg::glVertex(inner[j]);
            glEnd();

            glLineWidth(3);
            glBegin(GL_LINES);
            if(node[i].left_length >= avoid)
            {
                tkg::glVertex( inner[0] );
                tkg::glVertex( inner[1] );
            }
            if(node[i].right_length >= avoid)
            {
                tkg::glVertex( inner[2] );
                tkg::glVertex( inner[3] );
            }
            glEnd();
            glLineWidth(1);
        }
    }

    // Edge -----------------------------------------
    for(int s = 0; s < 2; ++s)
    {
        glLineWidth(1 + s*2);
        glBegin(GL_LINES);
        for(uint i=1; i<node.size(); i++)
        {
            if(s ^ (node[i-1].velocity > 0.5 ? 1 : 0)) continue;
            glColor4dv( edge_color[(node[i-1].search.enable_searching?1:0)].rgba );
            tkg::glVertex( POS(node[i-1]) );
            tkg::glVertex( POS(node[i  ]) );
        }
        glEnd();
    }

    // Seamless -------------------------------------
    glColor4dv(node_color[1].rgba);
    glBegin(GL_LINES);
    for(uint i=1; i<node.size(); i++)
    {
        if( node[i].seamless )
        {
            tkg::Point3 v,w;
            v = POS(node[i-1]) - POS(node[i]);
            v = v.unit();
            w = v.rotateZ(tkg::pi/2);
            tkg::glVertex( POS(node[i]) + v * node[i].seamless_length + w * 0.5 );
            tkg::glVertex( POS(node[i]) + v * node[i].seamless_length - w * 0.5 );
        }
    }
    glEnd();
    glLineWidth(1);


    // Node -----------------------------------------
    glPointSize(9);
    glBegin(GL_POINTS);
    for(uint i=0; i<node.size(); i++)
    {
        glColor4dv(node_color[(node[i].seamless?1:0)].rgba);
        tkg::glVertex( POS(node[i]) );
    }
    glEnd();

    double size = scale *  text_size;
    double ypos = scale * (text_size + 0.05);

    glColor4dv(text_color.rgba);
    for(uint i=0; i<node.size(); i++)
    {
        tkg::glString(tkg::strf("%d",i), POS(node[i]) + tkg::Point3(0, ypos, 0), size, 0, 0);
    }
}

void RouteEdit::drawReference()
{
    for(uint j=0; j<reference_node.size(); j++)
    {
        glColor4dv(reference_color[j].rgba);

        glLineWidth(1);
        glBegin(GL_LINES);
        for(uint i=1; i<reference_node[j].size(); i++)
        {
            tkg::glVertex( reference_node[j][i-1] );
            tkg::glVertex( reference_node[j][i  ] );
        }
        glEnd();

        glPointSize(7);
        glBegin(GL_POINTS);
        for(uint i=0; i<reference_node[j].size(); i++)
        {
            tkg::glVertex( reference_node[j][i] );
        }
        glEnd();

        for(uint i=0; i<reference_node[j].size(); i++)
        {
            tkg::glString(tkg::strf("%d",i), reference_node[j][i] + tkg::Point3(0,1,0), 0.5, 0, 0);
        }
    }
}


bool RouteEdit::selected()
{
    return (node_select != -1);
}

void RouteEdit::undo()
{
    if(log.empty()) return;
    LogElement g = log.back(); log.pop_back();

    switch(g.type)
    {
        case PUSH:
            node.pop_back();
            break;

        case MOVE:
            node[g.select] = g.data;
            break;

        case DELETE:
            node.insert(node.begin() + g.select, g.data);
            break;

        case INSERT:
            node.erase(node.begin() + g.select);
            break;

        default:
            break;
    }

    update = true;
}

void RouteEdit::reset()
{
    node_select = -1;
    edge_select = -1;

    if(tmplog.type != NONE)
    {
        log.push_back(tmplog);
        tmplog.type = NONE;
    }
}

void RouteEdit::set(double x, double y, double r)
{
    node_select = -1;
    edge_select = -1;

    tkg::Point3 pos(x,y,0);
    double mindist = tkg::inf;

    // node select ----------------------------------------

    for(uint i=0; i<node.size(); i++)
    {
        double dist = (POS(node[i]) - pos).abs();
        if( dist > r ) continue;
        if( dist < mindist )
        {
            node_select = i;
            mindist     = dist;
        }
    }

    if(0 <= node_select && node_select < (int)node.size())
    {
        tmplog.type   = MOVE;
        tmplog.select = node_select;
        tmplog.data   = node[node_select];
        return;
    }

    // edge select ----------------------------------------

    for(uint i=0; i+1<node.size(); i++)
    {
        std::vector<tkg::Point3> box = inner_box(i);
        for(int j=0; j<4; j++)
        {
            tkg::Point3 u = box[j+1] - box[j];
            tkg::Point3 v =      pos - box[j];

            double dot = ( u.x * v.x + u.y * v.y ) / (u.abs() * u.abs());
            if(dot < 0.0 || 1.0 < dot) continue;

            double dist = fabs( u.x * v.y - u.y * v.x ) / u.abs();
            if( dist > r ) continue;
            if( dist < mindist )
            {
                edge_select = i * 10 + j;
                mindist     = dist;
            }
        }
    }

    if(edge_select != -1 && 0 <= (edge_select/10) && (edge_select/10)+1 < (int)node.size())
    {
        tmplog.type   = MOVE;
        tmplog.select = edge_select/10;
        tmplog.data   = node[edge_select/10];
        return;
    }

    // seamless select ------------------------------------

    for(uint i=1; i<node.size(); i++)
    {
        if(!node[i].seamless) continue;

        tkg::Point3 u,v,w,s,t;
        v = POS(node[i-1]) - POS(node[i]);
        v = v.unit();
        w = v.rotateZ(tkg::pi/2);
        s = POS(node[i]) + v * node[i].seamless_length + w * 0.5;
        t = POS(node[i]) + v * node[i].seamless_length - w * 0.5;
        u = t   - s;
        v = pos - s;

        double dot = ( u.x * v.x + u.y * v.y ) / (u.abs() * u.abs());
        if(dot < 0.0 || 1.0 < dot) continue;

        double dist = fabs( u.x * v.y - u.y * v.x ) / u.abs();
        if( dist > r ) continue;
        if( dist < mindist )
        {
            edge_select = i * 10 + 5;
            mindist     = dist;
        }
    }

    if(edge_select != -1 && 1 <= (edge_select/10) && (edge_select/10) < (int)node.size())
    {
        tmplog.type   = MOVE;
        tmplog.select = edge_select/10;
        tmplog.data   = node[edge_select/10];
        return;
    }
}

void RouteEdit::push(double x, double y, double r)
{
    tkg::Point3 pos(x,y,0);
    int    target  = -1;
    double mindist = tkg::inf;

    // delete mode -----------------------------

    for(uint i=0; i<node.size(); i++)
    {
        double dist = (POS(node[i]) - pos).abs();
        if( dist > r) continue;
        if( dist < mindist )
        {
            target  = i;
            mindist = dist;
        }
    }

    if(target != -1)
    {
        if(!log.empty()) log.pop_back();

        tmplog.type   = DELETE;
        tmplog.select = target;
        tmplog.data   = node[target];
        log.push_back( tmplog );
        tmplog.type = NONE;

        node.erase(node.begin() + target);

        update = true; return;
    }

    // insert mode -----------------------------

    for(uint i=1; i<node.size(); i++)
    {
        tkg::Point3 u = POS(node[i]) - POS(node[i-1]);
        tkg::Point3 v = pos          - POS(node[i-1]);

        double dot = ( u.x * v.x + u.y * v.y ) / u.abs() / u.abs();
        if(dot < 0.0 || 1.0 < dot) continue;

        double dist = fabs( u.x * v.y - u.y * v.x ) / u.abs();
        if( dist > r) continue;
        if( dist < mindist )
        {
            target  = i;
            mindist = dist;
        }
    }

    if(target != -1)
    {
        tmplog.type   = INSERT;
        tmplog.select = target;
        log.push_back( tmplog );
        tmplog.type = NONE;

        WayPointData waypoint;
        waypoint.x                       = pos.x;
        waypoint.y                       = pos.y;
        waypoint.velocity                = node[target-1].velocity;
        waypoint.type                    = WayPointData::TYPE_DEFAULT;
        waypoint.seamless                = node[target-1].seamless;
        waypoint.seamless_length         = node[target-1].seamless_length;
        waypoint.right_length            = node[target-1].right_length;
        waypoint.left_length             = node[target-1].left_length;
        waypoint.front_length            = node[target-1].front_length;
        waypoint.rear_length             = node[target-1].rear_length;
        waypoint.search.enable_searching = node[target-1].search.enable_searching;
        waypoint.avoid.count             = 0;
        node.insert(node.begin() + target, waypoint);

        update = true;
        return;
    }

    // push mode -------------------------------

    tmplog.type = PUSH;
    log.push_back( tmplog );
    tmplog.type = NONE;

    WayPointData waypoint;
    waypoint.x                       = pos.x;
    waypoint.y                       = pos.y;
    waypoint.velocity                = 0.7;
    waypoint.type                    = WayPointData::TYPE_DEFAULT;
    waypoint.seamless                = !node.empty();
    waypoint.seamless_length         = 0.5;
    waypoint.right_length            = 2.0;
    waypoint.left_length             = 2.0;
    waypoint.front_length            = 0.0;
    waypoint.rear_length             = 0.0;
    waypoint.search.enable_searching = 0;
    waypoint.avoid.count             = 0;   
    node.push_back( waypoint );

    update = true;
    return;

    // -----------------------------------------
}

void RouteEdit::move(double x, double y)
{
    if(0 <= node_select && node_select < (int)node.size())
    {
        node[node_select].x = x;
        node[node_select].y = y;
        update = true;
    }

    if(edge_select != -1 && 0 <= (edge_select/10) && (edge_select/10)+1 < (int)node.size() && edge_select % 10 < 4)
    {
        int i = edge_select / 10;
        tkg::Point3 pos(x,y,0);
        tkg::Point3 u = POS(node[i]) - POS(node[i+1]);
        tkg::Point3 v = pos          - POS(node[i+1]);

        if((edge_select % 10) % 2) u = u.rotateZ(tkg::pi/2);

        double dist = (u.x * v.y - u.y * v.x) / u.abs();
        switch(edge_select % 10)
        {
            case 0: node[i].left_length  = std::max(0.0, -dist); break;
            case 1: node[i].front_length = std::max(0.0,  dist); break;
            case 2: node[i].right_length = std::max(0.0,  dist); break;
            case 3: node[i].rear_length  = std::max(0.0, -dist-u.abs()); break;
        }

        update = true;
    }

    if(edge_select != -1 && 1 <= (edge_select/10) && (edge_select/10) < (int)node.size() && edge_select % 10 == 5)
    {
        int i = edge_select / 10;
        tkg::Point3 pos(x,y,0);
        tkg::Point3 u = POS(node[i-1]) - POS(node[i]);
        tkg::Point3 v = pos            - POS(node[i]);

        double dist = (u.x * v.x + u.y * v.y) / u.abs();
        node[i].seamless_length = std::max(0.0, dist);

        update = true;
    }
}

void RouteEdit::change(double s)
{
    if(node_select<0 || (int)node.size()<=node_select) return;

    if(s < 0) node[node_select].velocity = 0.3;
    if(s > 0) node[node_select].velocity = 0.7;

    update = true;
}
