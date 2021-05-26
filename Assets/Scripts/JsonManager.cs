using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class JsonManager : MonoBehaviour
{
    [SerializeField] private string streaminAssetsPath;
    string jsonText;

    private IEnumerator Start()
    {
        streaminAssetsPath = Path.Combine(Application.streamingAssetsPath, "gameData.json");
        StartCoroutine(LoadJson());
        yield return null;
    }

    IEnumerator LoadJson()
    {
        jsonText = string.Empty;
   

        if (File.Exists(streaminAssetsPath))
        {
            jsonText = File.ReadAllText(streaminAssetsPath);
            JsonData jsonData = JsonUtility.FromJson<JsonData>(jsonText);

      

            foreach (var conversation in jsonData.conversation)
            {
                GameInfo.Instance.conversationList.Add(conversation.key, conversation);
            }

            foreach (var character in jsonData.characterInfo)
            {
                GameInfo.Instance.characterInfos.Add(character.name, character);
            }
        }

        yield return null;
    }

}
