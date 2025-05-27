#include "mxl2irp.h"

//  xml elements have three types:
//  PARENT : <parent>
//               <child/>
//           </parent>
//  Parents can only store child elements like variables or self-closing elements.
//  
//  VARIABLE : <var>brother ewww</var>
//  Variables can only store plain text in them slot.
//  
//  SELF_CLOSING : <solo/>
//  self-closing elements can only store additional attributes.
//  
//  All xml types can have indeterminates number of attributes.
//  key="value"

//  ## Bar Lines
//  -- IREAL PRO --
//  
//  | single bar line
//  [ opening double bar line
//  ] closing double bar line
//  { opening repeat bar line
//  } closing repeat bar line
//  Z Final thick double bar line
//  
//  Example: {C |A- |D- |G7 }[C |A- |D- |G7 Z
//  
//  -- MUSICXML --
//  
//  <barline> XML::PARENT
//      https://www.w3.org/2021/06/musicxml40/musicxml-reference/elements/barline/
//  
//  SLOTS:
//  <bar-style> XML::VARIABLE (optional)
//      https://www.w3.org/2021/06/musicxml40/musicxml-reference/data-types/bar-style/
//      most possible values :
//          heavy, heavy-heavy, heavy-light, light-heavy, light-light, regular
//  <repeat> XML::SELF_CLOSING (optional)
//      attributes :
//           (required) key: direction , value: backward | forward
static char parse_barline(xmlNode* measure)
{
    return '\0';
};

static void parse_measure(xmlNode* e, irp_measure* target)
{

};

int mxl2irp_load_parser()
{
    return 0;
}

int mxl2irp_get_url(mxl2irp_convert_params* params, char* urlBuffer)
{
    return 0;
};
