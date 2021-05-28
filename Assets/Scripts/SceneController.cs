using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
public class SceneController : GenericSingleton<SceneController>
{
    public GameObject loading;
    public Image progressBar;

    protected override void Awake()
    {
        base.Awake();
        loading = transform.GetChild(0).gameObject;
        progressBar = loading.transform.GetChild(0).GetComponent<Image>();
    }

    public void Start()
    {
        loading.SetActive(false);
        progressBar.fillAmount = 0f;
    }
    public void GoToScene(string sceneName)
    {
        loading.SetActive(true);
        StartCoroutine(StartLoad("Game"));
    }

    IEnumerator StartLoad(string sceneName)
    {
        AsyncOperation asyncOperation = SceneManager.LoadSceneAsync(sceneName);
        asyncOperation.allowSceneActivation = false;

        while (!asyncOperation.isDone)
        {
            Debug.Log(asyncOperation.progress);
            progressBar.fillAmount = Mathf.Clamp01(asyncOperation.progress);

            if (asyncOperation.progress >= 0.9f)
            {
                asyncOperation.allowSceneActivation = true;

            }

            yield return null;
        }

        loading.SetActive(false);
        progressBar.fillAmount = 0f;

        yield return null;
    }
}
