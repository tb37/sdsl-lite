/*!
 * \author: Matthias Petri
 */
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

string create_html_header(const char* file_name)
{
    std::stringstream jsonheader;
    jsonheader
            << "<html>" << endl
            << "   <head>" << endl
            << "      <meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\">" << endl
            << "      <title>" << file_name << "</title>" << endl
            << "      <script src=\"http://d3js.org/d3.v2.js\"></script>" << endl
            << "      <style type=\"text/css\">" << endl
            << "          path { stroke: #000; stroke-width: 0.8; cursor: pointer; }" << endl
            << "          text { font: 11px sans-serif; cursor: pointer; }" << endl
            << "          body { width: 900; margin: 0 auto; }" << endl
            << "          h1 { text-align: center; margin: .5em 0; }" << endl
            << "          #breadcrumbs { display: none; }" << endl
            << "          svg { font: 10px sans-serif; }" << endl
            << "       </style>" << endl
            << "    </head>" << endl
            << "</head>" << endl
            << "<body>" << endl
            << "    <div id=\"chart\"></div>" << endl;
    return jsonheader.str();
}

string create_js_body(const std::string& jsonsize)
{
    std::stringstream jsonbody;
    jsonbody
            << "<script type=\"text/javascript\">" << endl
            << ""
            "var w = 800,\n"
            "    h = w,\n"
            "    r = w / 2,\n"
            "    x = d3.scale.linear().range([0, 2 * Math.PI]),\n"
            "    y = d3.scale.pow().exponent(1.3).domain([0, 1]).range([0, r]),\n"
            "    p = 5,\n"
            "    color = d3.scale.category20c(),\n"
            "    duration = 1000;\n"
            "\n"
            "var vis = d3.select(\"#chart\").append(\"svg:svg\")\n"
            "    .attr(\"width\", w + p * 2)\n"
            "    .attr(\"height\", h + p * 2)\n"
            "    .append(\"g\")\n"
            "    .attr(\"transform\", \"translate(\" + (r + p) + \",\" + (r + p) + \")\");\n"
            "\n"
            "vis.append(\"p\")\n"
            "    .attr(\"id\", \"intro\")\n"
            "    .text(\"Click to zoom!\");\n"
            "\n"
            "var partition = d3.layout.partition()\n"
            "    .sort(null)\n"
            "    .size([2 * Math.PI, r * r])\n"
            "    .value(function(d) { return d.size; });\n"
            "\n"
            "var arc = d3.svg.arc()\n"
            "    .startAngle(function(d) { return Math.max(0, Math.min(2 * Math.PI, x(d.x))); })\n"
            "    .endAngle(function(d) { return Math.max(0, Math.min(2 * Math.PI, x(d.x + d.dx))); })\n"
            "    .innerRadius(function(d) { return Math.max(0, d.y ? y(d.y) : d.y); })\n"
            "    .outerRadius(function(d) { return Math.max(0, y(d.y + d.dy)); });\n"
            "\n"
            "            " << endl
            << "var spaceJSON = " << jsonsize << ";" << endl << endl
            << "\n"
            "\n"
            "  var nodes = partition.nodes(spaceJSON);\n"
            "\n"
            "  var path = vis.selectAll(\"path\").data(nodes);\n"
            "  path.enter().append(\"path\")\n"
            "      .attr(\"id\", function(d, i) { return \"path-\" + i; })\n"
            "      .attr(\"d\", arc)\n"
            "      .attr(\"fill-rule\", \"evenodd\")\n"
            "      .style(\"fill\", colour)\n"
            "      .on(\"click\", click);\n"
            "\n"
            "  path.append(\"title\").text(function(d) { return 'class name: ' + d.class_name + '\\nmember_name: ' + d.name + '\\n size: ' + sizeMB(d) });\n"
            "\n"
            "  var text = vis.selectAll(\"text\").data(nodes);\n"
            "  var textEnter = text.enter().append(\"text\")\n"
            "      .style(\"opacity\", 1)\n"
            "      .style(\"fill\", function(d) {\n"
            "        return brightness(d3.rgb(colour(d))) < 125 ? \"#eee\" : \"#000\";\n"
            "      })\n"
            "      .attr(\"text-anchor\", function(d) {\n"
            "        return x(d.x + d.dx / 2) > Math.PI ? \"end\" : \"start\";\n"
            "      })\n"
            "      .attr(\"dy\", \".2em\")\n"
            "      .attr(\"transform\", function(d) {\n"
            "        var multiline = (d.name || \"\").split(\" \").length > 1,\n"
            "            angle = x(d.x + d.dx / 2) * 180 / Math.PI - 90,\n"
            "            rotate = angle + (multiline ? -.5 : 0);\n"
            "        return \"rotate(\" + rotate + \")translate(\" + (y(d.y) + p) + \")rotate(\" + (angle > 90 ? -180 : 0) + \")\";\n"
            "      })\n"
            "      .on(\"click\", click);\n"
            "\n"
            "  textEnter.append(\"title\").text(function(d) { return 'class name: ' + d.class_name + '\\nmember_name: ' + d.name + '\\n size: ' + sizeMB(d) });\n"
            "\n"
            "  textEnter.append(\"tspan\")\n"
            "      .attr(\"x\", 0)\n"
            "      .text(function(d) { return d.dx < 0.05 ? \"\" : d.depth ? d.name.split(\" \")[0] : \"\"; });\n"
            "  textEnter.append(\"tspan\")\n"
            "      .attr(\"x\", 0)\n"
            "      .attr(\"dy\", \"1em\")\n"
            "      .text(function(d) { return d.dx < 0.05 ? \"\" : d.depth ? d.name.split(\" \")[1] || \"\" : \"\"; });\n"
            "\n"
            "  function click(d) {\n"
            "    path.transition()\n"
            "      .duration(duration)\n"
            "      .attrTween(\"d\", arcTween(d));\n"
            "\n"
            "    // Somewhat of a hack as we rely on arcTween updating the scales.\n"
            "    text\n"
            "      .style(\"visibility\", function(e) {\n"
            "        return isParentOf(d, e) ? null : d3.select(this).style(\"visibility\");\n"
            "      })\n"
            "      .transition().duration(duration)\n"
            "      .attrTween(\"text-anchor\", function(d) {\n"
            "        return function() {\n"
            "          return x(d.x + d.dx / 2) > Math.PI ? \"end\" : \"start\";\n"
            "        };\n"
            "      })\n"
            "      .attrTween(\"transform\", function(d) {\n"
            "        var multiline = (d.name || \"\").split(\" \").length > 1;\n"
            "        return function() {\n"
            "          var angle = x(d.x + d.dx / 2) * 180 / Math.PI - 90,\n"
            "              rotate = angle + (multiline ? -.5 : 0);\n"
            "          return \"rotate(\" + rotate + \")translate(\" + (y(d.y) + p) + \")rotate(\" + (angle > 90 ? -180 : 0) + \")\";\n"
            "        };\n"
            "      })\n"
            "      .style(\"opacity\", function(e) { return isParentOf(d, e) ? 1 : 1e-6; })\n"
            "      .each(\"end\", function(e) {\n"
            "        d3.select(this).style(\"visibility\", isParentOf(d, e) ? null : \"hidden\");\n"
            "      });\n"
            "  }\n"
            "\n"
            "\n"
            "function sizeMB(d) {\n"
            "//  if (d.children) {\n"
            "//    var sum = calcSum(d);\n"
            "//    return (sum / (1024*1024)).toFixed(2) + 'MB';\n"
            "//  } else {\n"
            "    return (d.size / (1024*1024)).toFixed(2) + 'MB';\n"
            "//  }\n"
            "}\n"
            "\n"
            "function calcSum(d) {\n"
            "  if(d.children) {\n"
            "    var sum = 0;\n"
            "    function recurse(d) {\n"
            "      if(d.children) d.children.forEach( function(child) { recurse(child); } );\n"
            "      else sum += d.size;\n"
            "    }\n"
            "    recurse(d,sum);\n"
            "    console.log(sum);\n"
            "    console.log(d.children);\n"
            "    return sum;\n"
            "  } else {\n"
            "    console.log(d.size);\n"
            "    return d.size;\n"
            "  }\n"
            "}\n"
            "\n"
            "function isParentOf(p, c) {\n"
            "  if (p === c) return true;\n"
            "  if (p.children) {\n"
            "    return p.children.some(function(d) {\n"
            "      return isParentOf(d, c);\n"
            "    });\n"
            "  }\n"
            "  return false;\n"
            "}\n"
            "\n"
            "function colour(d) {\n"
            "    return color(d.name);\n"
            "}\n"
            "\n"
            "// Interpolate the scales!\n"
            "function arcTween(d) {\n"
            "  var my = maxY(d),\n"
            "      xd = d3.interpolate(x.domain(), [d.x, d.x + d.dx]),\n"
            "      yd = d3.interpolate(y.domain(), [d.y, my]),\n"
            "      yr = d3.interpolate(y.range(), [d.y ? 20 : 0, r]);\n"
            "  return function(d) {\n"
            "    return function(t) { x.domain(xd(t)); y.domain(yd(t)).range(yr(t)); return arc(d); };\n"
            "  };\n"
            "}\n"
            "\n"
            "// Interpolate the scales!\n"
            "function arcTween2(d) {\n"
            "  var xd = d3.interpolate(x.domain(), [d.x, d.x + d.dx]),\n"
            "      yd = d3.interpolate(y.domain(), [d.y, 1]),\n"
            "      yr = d3.interpolate(y.range(), [d.y ? 20 : 0, radius]);\n"
            "  return function(d, i) {\n"
            "    return i\n"
            "        ? function(t) { return arc(d); }\n"
            "        : function(t) { x.domain(xd(t)); y.domain(yd(t)).range(yr(t)); return arc(d); };\n"
            "  };\n"
            "}\n"
            "\n"
            "function maxY(d) {\n"
            "  return d.children ? Math.max.apply(Math, d.children.map(maxY)) : d.y + d.dy;\n"
            "}\n"
            "\n"
            "// http://www.w3.org/WAI/ER/WD-AERT/#color-contrast\n"
            "function brightness(rgb) {\n"
            "  return rgb.r * .299 + rgb.g * .587 + rgb.b * .114;\n"
            "}\n"
            "\n"
            "click(  nodes[0] );" << endl
            << "</script>" << endl
            << "</body>" << endl
            << "</html>" << endl;
    return jsonbody.str();
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Usage: "<<argv[0]<< " structure_file" << endl;
        cout << " (1) Reads in a structure file generated by a call of" << endl;
        cout << "     write_structure<JSON_FORMAT>o,...) for an object o." << endl;
        cout << " (2) Outputs a html-file visualizing the data to stdout." << endl;
    } else {
        ifstream in(argv[1]);
        string json_input((std::istreambuf_iterator<char>(in)),
                          std::istreambuf_iterator<char>());

        string html = "";
        html += create_html_header(argv[1]);
        html += create_js_body(json_input);
        cout << html;
    }
}
