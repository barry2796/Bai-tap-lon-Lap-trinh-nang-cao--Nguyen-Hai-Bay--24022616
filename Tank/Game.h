class Game{
public;
SDL_Renderer* renderer;
SDL_Window* window;
bool chayCtrinh;
    //Các lệnh hàm Game viết ở đây
    Game (){
        chayCtrinh=true; //Cho game chạy, nếu ko can thiệp gì chắc là chạy vĩnh viễn luôn
        if(SDL_Init(SDL_INIT_VIDEO)<0){
        cerr<<"Lỗi ròi, khong chạy được"<<SDL_GetError()<<endl;
        chayCtrinh=false;
        }
        if(!window){
        cerr<<"Lỗi ròi, khong chạy được"<<SDL_GetError()<<endl;
        chayCtrinh=false;
        }
        if(!renderer){
        cerr<<"Lỗi ròi, khong chạy được"<<SDL_GetError()<<endl;
        chayCtrinh=false;
        }

    }
    //Vẽ lên màn hình ở đây
    void render(){
    }
    //Bật tắt game ở đây
    void chay(){
    }
    ~Game(){}// Cái này vẫn chưa biết để làm gì, lmao :(((
}
