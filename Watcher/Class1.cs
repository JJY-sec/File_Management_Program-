using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices; //Guid();

namespace DirectoryWatcher
{
    [Guid("1678228D-AD30-4F67-811B-D1F965266F0E")] //dll만들때 필요
    public interface Watcherdll //dll에 쓰임
    {
        void InitWatcher();
        string Send_signal(string signal);
    }
    [Guid("EA1BF021-EEBB-4024-A9A9-779AF315575F")]
    public class Watcher : Watcherdll
    {
        public void InitWatcher() //.cs로 실행하려면 Watcherdll 주석처리, public static Main()으로 바꿔야 함
        {
            FileSystemWatcher watcher = new FileSystemWatcher(@"C:\"); //경로
            watcher.EnableRaisingEvents = true;

            watcher.NotifyFilter = NotifyFilters.DirectoryName
                | NotifyFilters.FileName
                | NotifyFilters.LastWrite
                | NotifyFilters.Size;

            watcher.Changed += File_Changed;
            watcher.Created += File_Created;
            watcher.Changed += File_Deleted;
            watcher.Renamed += File_Renamed;

            watcher.Filter = ""; //모든 타입의 파일
            watcher.IncludeSubdirectories = true; //하위 디렉토리 감시
            watcher.EnableRaisingEvents = true; //구성요소 활성화

            //watcher.WaitForChanged(WatcherChangeTypes.All);
            //Console.WriteLine("Press any button to exit.");
            Console.ReadLine();
        }
        public static void File_Changed(object source, FileSystemEventArgs e)
        {
            if (e.ChangeType != WatcherChangeTypes.Changed)
            {
                return;
            }
            string signal = $"Changed{e.FullPath}";
            //Console.WriteLine(signal);
            Watcher.temp = new Watcher();
            temp.Send_signal(signal);
        }
        public static void File_Created(object source, FileSystemEventArgs e)
        {
            string signal = $"Created{e.FullPath}";
            //Console.WriteLine(signal);
            Watcher temp = new Watcher();
            temp.Send_signal(signal);
        }
        public static void File_Deleted(object source, FileSystemEventArgs e)
        {
            string signal = $"Deleted{e.FullPath}";
            //Console.WriteLine(signal);
            Watcher temp = new Watcher();
            temp.Send_signal(signal);
        }
        public static void File_Renamed(object source, RenamedEventArgs e)
        {
            string signal = $"Renamed{e.OldFullPath}||{e.FullPath}";
            //string oldvalue_newvalue = e.OldFullPath+"||"+e.FullPath;
            //Console.WriteLine(oldvalue_newvalue);
            Watcher temp = new Watcher();
            temp.Send_signal(signal);
        }
        //각 static 함수들에서 발생한 이벤트를 FullPath앞에 붙여서 받음. C++에서 if문이나 case로 각 이벤트 구별
        //C++ 파일에서 .substr(7)로 뒤의 파일경로 얻을 수 있음.
        public static string Send_signal(string signal) //c++에 string 반환
        {
            //Console.WriteLine(signal);
            return signal;
        }
    }
}
