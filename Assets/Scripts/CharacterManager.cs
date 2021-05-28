using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class CharacterManager : MonoBehaviour
{
    public Text nameText;
    public Transform character;
    [SerializeField] private GameObject[] characters;

    private void Start()
    {
        GameInfo.Instance.CharacterIndex = 0;
        characters = new GameObject[character.childCount - 1];

        for (int i = 0; i < characters.Length; i++)
        {
            characters[i] = character.GetChild(i).gameObject;
            string name = characters[i].name.Remove(0, 4);
            GameInfo.Instance.characterNames.Add(name, i);
        }

        SetName();
    }

    public void ClickLeftButton()
    {
        characters[GameInfo.Instance.CharacterIndex].SetActive(false);
        GameInfo.Instance.CharacterIndex--;

        if (GameInfo.Instance.CharacterIndex < 0)
        {
            GameInfo.Instance.CharacterIndex = characters.Length - 1;
        }

        characters[GameInfo.Instance.CharacterIndex].SetActive(true);

        SetName();
    }

    public void ClickRightButton()
    {
        characters[GameInfo.Instance.CharacterIndex].SetActive(false);
        GameInfo.Instance.CharacterIndex++;

        if (GameInfo.Instance.CharacterIndex >= characters.Length)
        {
            GameInfo.Instance.CharacterIndex = 0;
        }

        characters[GameInfo.Instance.CharacterIndex].SetActive(true);

        SetName();
    }

    private void SetName()
    {
        string name = characters[GameInfo.Instance.CharacterIndex].name;
        string[] words = name.Split('_');
        nameText.text = $"{words[2]}{words[3]}";
    }

    public void GoToGame()
    {
        SceneController.Instance.GoToScene("Game");
       
    }

}
