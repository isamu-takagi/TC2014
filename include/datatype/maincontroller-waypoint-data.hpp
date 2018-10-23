/*
 * main-controllerでmasterとslaveの間で使われるwaypointの構造体。
 * route-editorでもこのデータ型を使う必要があるため、includeの中に定義した
 *
 * 2014-10-02 MTM
 *
 */

#ifndef MAINCONTROLLER_WAYPOINTDATA_HPP
#define MAINCONTROLLER_WAYPOINTDATA_HPP

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================
//
// include
//
//========================================================
//c
#include<cstdlib>
#include<vector>
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>

//========================================================
//
// ユーティリティ関数宣言
//
//========================================================

//指定された文字で文字列を分割して返す。
//[in]  string         input          入力文字列
//[in]  char           separate_char  分割する文字
//[in]  bool           ignore_empty   trueにすると、戻り値に空の文字列を入れない（デフォルトでtrue）
//[in]  bool           trimming       trueにすると、戻り値に各要素の先頭と末尾のスペースを削除（デフォルトでtrue）
//[out] vector<string> (戻り値)        分割結果
inline std::vector< std::string >
splitString( const std::string& input , const char separate_char )
{

  //戻り値となる文字列ベクタの定義
  std::vector< std::string > ret;

  //inputを前から捜査→分割文字ならretに追加
  std::string current_str = "";
  for( size_t i=0; i<input.size(); ++i )
  {
     if( input[i] == separate_char )
     {
       ret.push_back( current_str );
       current_str.clear();
     }
     else
     {
        current_str.push_back( input[i] );
     }
  }  
  ret.push_back( current_str );

  //retを戻す
  return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================
//
// WayPointDataForTargetSearchingの宣言
//
//========================================================
struct WayPointDataForTargetSearching
{
    //-----------------------------------------
    // コンストラクタ、デストラクタ
    //-----------------------------------------
    WayPointDataForTargetSearching();                                                 //デフォルトコンストラクタ
    WayPointDataForTargetSearching( const WayPointDataForTargetSearching& obj );  //コピーコンストラクタ（vector配列に代入するために必要）
    ~WayPointDataForTargetSearching();                                                //デストラクタ

    //-----------------------------------------
    // 文字列の読み込み、書き出し
    //-----------------------------------------
    void setString( const std::string& input_str ); //文字列から値をセットする
    std::string getString() const;                  //値から文字列を書き出す

    //-----------------------------------------
    // メンバ変数
    //-----------------------------------------
    //＊＊＊＊ここを編集＊＊＊＊
    //int   nanka;
    //int   teigi;
    //float shitene;
//    float right_length;
//    float left_length;

    bool enable_searching;
    bool is_back;
};

//-----------------------------------------
// コンストラクタ、デストラクタ
//-----------------------------------------
inline WayPointDataForTargetSearching::WayPointDataForTargetSearching():
    //＊＊＊＊ここを編集＊＊＊＊
    // nanka(0),
    // teigi(0),
    // shitene(1.0f)
    enable_searching(false),
    is_back(false)
{}

inline WayPointDataForTargetSearching::WayPointDataForTargetSearching( const WayPointDataForTargetSearching& obj ):
    //＊＊＊＊ここを編集＊＊＊＊
    // nanka( obj.nanka ),
    // teigi( obj.teigi ),
    // shitene( obj.shitene )
    enable_searching(obj.enable_searching),
    is_back(obj.is_back)
{}

inline WayPointDataForTargetSearching::~WayPointDataForTargetSearching()
{}

//-----------------------------------------
// 文字列の読み込み、書き出し
//-----------------------------------------
inline void
WayPointDataForTargetSearching::setString( const std::string& input_str )
{
    //「,」で分ける
    std::vector< std::string > split = splitString( input_str , ',' );
    const size_t split_size = split.size();

    //＊＊＊＊ここを編集＊＊＊＊
    //if( 0 < split_size ) nanka   = atoi( split[0].c_str() );
    //if( 1 < split_size ) teigi   = atoi( split[1].c_str() );
    //if( 2 < split_size ) shitene = atof( split[2].c_str() );
//    if( 0 < split_size ) right_length = atof( split[0].c_str() );
//    if( 1 < split_size ) left_length = atof( split[1].c_str() );

    if( 0 < split_size )
    {
        if( split[0] == "1"  ) enable_searching = true;
        if( split[0] == "0" ) enable_searching = false;
    }
    if( 1 < split_size )
    {
        if( split[1] == "1" ) is_back = true;
        if( split[1] == "0" ) is_back = false;
    }
}

inline std::string
WayPointDataForTargetSearching::getString() const
{
    std::stringstream ss;

    //＊＊＊＊ここを編集＊＊＊＊
    // ss << nanka << ",";
    // ss << teigi << ",";
    // ss << shitene;
//    ss << right_length << ",";
//    ss << left_length;

    ss << enable_searching << ",";
    ss << is_back;

    return ss.str();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================
//
// WayPointPropertyForObstacleAvoidingの宣言
//
//========================================================
struct WayPointPropertyForObstacleAvoiding
{
    //-----------------------------------------
    // コンストラクタ、デストラクタ
    //-----------------------------------------
    WayPointPropertyForObstacleAvoiding();                                                  //デフォルトコンストラクタ
    WayPointPropertyForObstacleAvoiding( const WayPointPropertyForObstacleAvoiding& obj );  //コピーコンストラクタ（vector配列に代入するために必要）
    ~WayPointPropertyForObstacleAvoiding();                                                 //デストラクタ

    //-----------------------------------------
    // 文字列の読み込み、書き出し
    //-----------------------------------------
    void setString( const std::string& input_str ); //文字列から値をセットする
    std::string getString() const;                  //値から文字列を書き出す

    //-----------------------------------------
    // メンバ変数
    //-----------------------------------------
    //＊＊＊＊ここを編集＊＊＊＊
    //int   nanka;
    //int   teigi;
    //float shitene;
    int count;

};

//-----------------------------------------
// コンストラクタ、デストラクタ
//-----------------------------------------
inline WayPointPropertyForObstacleAvoiding::WayPointPropertyForObstacleAvoiding():
    //＊＊＊＊ここを編集＊＊＊＊
    // nanka(0),
    // teigi(0),
    // shitene(1.0f)
    count(0)
{}

inline WayPointPropertyForObstacleAvoiding::WayPointPropertyForObstacleAvoiding( const WayPointPropertyForObstacleAvoiding& obj ):
    //＊＊＊＊ここを編集＊＊＊＊
    // nanka( obj.nanka ),
    // teigi( obj.teigi ),
    // shitene( obj.shitene )
count( obj.count)
{}

inline WayPointPropertyForObstacleAvoiding::~WayPointPropertyForObstacleAvoiding()
{}

//-----------------------------------------
// 文字列の読み込み、書き出し
//-----------------------------------------
inline void
WayPointPropertyForObstacleAvoiding::setString( const std::string& input_str )
{
    //「,」で分ける
    std::vector< std::string > split = splitString( input_str , ',' );
    const size_t split_size = split.size();

    //＊＊＊＊ここを編集＊＊＊＊
    //if( 0 < split_size ) nanka   = atoi( split[0].c_str() );
    //if( 1 < split_size ) teigi   = atoi( split[1].c_str() );
    //if( 2 < split_size ) shitene = atof( split[2].c_str() );
    if( 0 < split_size ) count = atoi( split[0].c_str() );
}

inline std::string
WayPointPropertyForObstacleAvoiding::getString() const
{
    std::stringstream ss;

    //＊＊＊＊ここを編集＊＊＊＊
    // ss << nanka << ",";
    // ss << teigi << ",";
    // ss << shitene;
    ss << count;
    return ss.str();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================
//
// WayPointDataの宣言
//
//========================================================
struct WayPointData
{

  //-----------------------------------------
  // enum宣言
  //-----------------------------------------
  enum Type
  {
    TYPE_DEFAULT,	       //デフォルトで用意されたウェイポイントである
    TYPE_TARGET_SEARCHING,     //人物の探索のために用意されたウェイポイントである
    TYPE_OBSTACLE_AVOIDING     //障害物回避のために用意されたウェイポイントである
  };

  //-----------------------------------------
  // コンストラクタ、デストラクタ
  //-----------------------------------------
  WayPointData();                           //デフォルトコンストラクタ
  WayPointData( const WayPointData& obj );  //コピーコンストラクタ（vector配列に代入するために必要）
  ~WayPointData();                          //デストラクタ

  //-----------------------------------------
  // 文字列の読み込み、書き出し
  //-----------------------------------------
  bool setString( const std::string& input_str ); //文字列から値をセットする
  std::string getString() const;                  //値から文字列を書き出す

  //-----------------------------------------
  // メンバ変数
  //-----------------------------------------
  //基本データ
  double x;        //ウェイポイントのx位置（GL座標系、単位[m]）
  double y;        //ウェイポイントのy位置（GL座標系、単位[m]）
  double velocity; //このウェイポイントを始点とする直線を走るときの、ロボットの速度[m/s]
  Type   type;     //どのマスターによってこのウェイポイントが用意されたか？

  //シームレス関係の値
  bool   seamless;         //次のウェイポイントをシームレスに曲がるかどうか（true:シームレスに曲がる、false:シームレスに曲がらない）
  double seamless_length;  //シームレスに曲がる場合、次のウェイポイントからこの数値だけ手前で次のlineコマンドを発行する（単位：[m]）

  float right_length;
  float left_length;
  float front_length;
  float rear_length;

  double pause_time;        //そのwaypointに到達した時に、一時停止する時間(sec)

  //各マスターのためのプロパティ値
  WayPointDataForTargetSearching  search;
  WayPointPropertyForObstacleAvoiding avoid;
};

//-----------------------------------------
// コンストラクタ、デストラクタ
//-----------------------------------------
inline WayPointData::WayPointData():
  x(0.0),
  y(0.0),
  velocity(0.5),
  type( WayPointData::TYPE_DEFAULT ),
  seamless(false),
  seamless_length(0.0),
  right_length(0.0),
  left_length(0.0),
  front_length(0.0),
  rear_length(0.0),
  pause_time(0.0),
  search(),
  avoid()
{}

inline WayPointData::WayPointData( const WayPointData& obj ):
  x( obj.x ),
  y( obj.y ),
  velocity( obj.velocity ),
  type( obj.type ),
  seamless( obj.seamless ),
  seamless_length( obj.seamless_length ),
  search( obj.search ),
  avoid( obj.avoid ),
  right_length( obj.right_length ),
  left_length( obj.left_length ),
  front_length( obj.front_length ),
  rear_length( obj.rear_length ),
  pause_time( obj.pause_time )
{}

inline WayPointData::~WayPointData()
{}

//-----------------------------------------
// 文字列の読み込み、書き出し
//-----------------------------------------
// ファイル書式
// （WayPointDataの各種パラメータ、カンマ区切り）|（WayPointDataForTargetSearchingの各種パラメータ、カンマ区切り）|（WayPointDataForObjectAvoidingの各種パラメータ、カンマ区切り）
//
inline bool
WayPointData::setString( const std::string& input_str )
{
  //まず「|」で大きく3つに分ける
  std::vector< std::string > split = splitString( input_str , '|' );

  if( split.size() < 3 )
  {
    return false;
  }
  else
  {
    //各マスター用プロパティの読み込み
    search.setString( split[1] );
    avoid.setString( split[2] );

    //WayPointDataset用プロパティの読み込み

    //「,」で分ける
    split = splitString( split[0] , ',' );
    const size_t split_size = split.size();

    //それぞれの値をメンバ変数にセット
    if( 0 < split_size ) x = atof( split[0].c_str() );
    if( 1 < split_size ) y = atof( split[1].c_str() );
    if( 2 < split_size ) velocity = atof( split[2].c_str() );
    if( 3 < split_size )
    {
      if( split[3] == "default"   ) type = TYPE_DEFAULT;
      if( split[3] == "searching" ) type = TYPE_TARGET_SEARCHING;
      if( split[3] == "avoiding"  ) type = TYPE_OBSTACLE_AVOIDING;
    }
    if( 4 < split_size )
    {
      if( split[4] == "seamless"  ) seamless = true;
      if( split[4] == "none-seamless" ) seamless = false;
    }
    if( 5 < split_size ) seamless_length = atof( split[5].c_str() );

    if( 6 < split_size ) right_length = atof( split[6].c_str() );
    if( 7 < split_size ) left_length = atof( split[7].c_str() );
    if( 8 < split_size ) front_length = atof( split[8].c_str() );
    if( 9 < split_size ) rear_length = atof( split[9].c_str() );
    if( 10 < split_size ) pause_time = atof( split[10].c_str() );

    return true;
  }
}

inline std::string
WayPointData::getString() const
{
    std::stringstream ss;

    ss << x << ",";
    ss << y << ",";
    ss << velocity << ",";

    switch( type )
    {
        default:
        case TYPE_DEFAULT:           ss << "default" << ","; break;
        case TYPE_TARGET_SEARCHING:  ss << "searching" << ","; break;
        case TYPE_OBSTACLE_AVOIDING: ss << "avoiding" << ","; break;
    }

   ss << ( seamless ? "seamless" : "none-seamless" ) << ",";

   ss << seamless_length << ",";


   ss << right_length << ",";
   ss << left_length << ",";
   ss << front_length << ",";
   ss << rear_length << ",";
   ss << pause_time << "|";

   ss << search.getString() << "|";
   ss << avoid.getString();

   return ss.str();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//========================================================
//
// その他ユーティリティ関数
//
//========================================================

//-----------------------------------------
//表示オペレータ
//-----------------------------------------
// WayPointDataをstd::coutで表示するためのオペレータ
// WayPointData wp; std::cout << wp << std::endl;で表示できる。
inline std::ostream& operator << ( std::ostream& os , const WayPointData& dataset )
{
    os << dataset.getString();
    return os;
}

//-----------------------------------------
//ファイル読み書き
//-----------------------------------------
// waypointファイルを読み込み、std::vector< WayPointData >に代入する。
//　戻り値は終了ステータス
//　　 0：正常終了
//　  1:ファイルは開けたが、有効なwaypointが１つも読み込めなかった
//  -1:ファイルが開けない
inline int
readWayPointFile( const std::string& file_name , std::vector< WayPointData >& output )
{
    std::ifstream ifs;
    ifs.open( file_name.c_str() );

    if( !ifs )
    {
        std::cerr << "\033[31m\033[1m[readWaypointFile()]failed to open waypoint file \"" << file_name << "\"!!\033[0m\033[39m\n";
        return -1;
    }
    else
    {
        std::string line;

        while( std::getline( ifs , line ) )
        {
            //コメントを消す
            std::string::size_type comment_pos = line.find("#");
            if( comment_pos != std::string::npos )
            {
                line.erase( comment_pos );
            }

            //読み込み
            WayPointData wp;
            if( wp.setString( line ) )
            {
                output.push_back( wp );
            }
        }

        if( output.empty() )
        {
            std::cout << "\033[31m\033[1m[readWaypointFile()]cannot find any waypoints in " << file_name << "!!\033[0m\033[39m\n";
            return 1;
        }
        else
        {
            std::cout << "[readWaypointFile()]success to read waypoint file. found " << output.size() << " waypoints.\n";
            return 0;
        }
    }
}

// waypointファイルに、std::vector< WayPointData >の内容を書き出す。
//　戻り値は終了ステータス
//　　 0：正常終了
//  -1:ファイルが開けない
inline int
writeWayPointFile( const std::string& file_name , const std::vector< WayPointData >& input )
{
    std::ofstream ofs;
    ofs.open( file_name.c_str() );

    if( !ofs )
    {
        std::cerr << "\033[31m\033[1m[writeWaypointFile()]failed to open waypoint file \"" << file_name << "\"!!\033[0m\033[39m\n";
        return -1;
    }
    else
    {
        for( size_t i=0; i<input.size(); ++i )
        {
            ofs << input[i].getString() << std::endl;
        }

        std::cout << "[readWaypointFile()]success to write waypoint file. ( filename : \033[1m" << file_name << "\033[0m )\n";

        return 0;
    }
}


#endif // MAINCONTROLLER_WAYPOINTDATA_HPP
