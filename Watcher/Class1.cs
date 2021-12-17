using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices; //Guid();

namespace DirectoryWatcher
{
    [Guid("1678228D-AD30-4F67-811B-D1F965266F0E")] //regasm으로 dll만들때 필요
    public interface Watcherdll
    {
        void initWatcher();
    }
    [Guid("EA1BF021-EEBB-4024-A9A9-779AF315575F")]
    class Watcher : Watcherdll
    {
        public void initWatcher()
        {
            FileSystemWatcher watcher = new FileSystemWatcher(@"C:\");
            //watcher = new FileSystemWatcher(directoryPath, "*.*");
            watcher.EnableRaisingEvents = true;

            watcher.NotifyFilter = NotifyFilters.DirectoryName
                | NotifyFilters.FileName
                | NotifyFilters.LastWrite
                | NotifyFilters.Size;

            watcher.Changed += File_Changed;
            watcher.Created += File_Created;
            watcher.Deleted += File_Deleted;
            watcher.Renamed += File_Renamed;
            watcher.Error += Watcher_Error;

            watcher.Filter = "*.*";
            watcher.IncludeSubdirectories = true; //하위 디렉토리 감시
            watcher.EnableRaisingEvents = true; //구성요소 활성화

            //watcher.WaitForChanged(WatcherChangeTypes.All);
            //Console.WriteLine("Press any button to exit.");
            Console.ReadLine(); //없으면 실행중지됨
        }
        public static void File_Changed(object source, FileSystemEventArgs e)
        {
            if (e.ChangeType != WatcherChangeTypes.Changed)
            {
                return;
            }
            string value = e.FullPath;
            //Console.WriteLine(value);
        }
        public static void File_Created(object source, FileSystemEventArgs e)
        {
            string value = e.FullPath;
            //Console.WriteLine(value);
        }
        public static void File_Deleted(object source, FileSystemEventArgs e)
        {
            string value = e.FullPath;
            //Console.WriteLine(value);
        }
        public static void File_Renamed(object source, RenamedEventArgs e)
        {
            string oldvalue_newvalue = e.OldFullPath+"||"+e.FullPath;
            //Console.WriteLine(oldvalue);
            //Console.WriteLine(newvalue);
        }
        public static void Watcher_Error(object source, ErrorEventArgs e)
        {
            string error = "detected an error"; //GUI에 전달
            if (e.GetException().GetType() == typeof(InternalBufferOverflowException))
            {
                error = "overflow";
            }
        }
    }
}